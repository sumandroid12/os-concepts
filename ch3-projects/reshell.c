#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 100
int fd_output, fd_input, fd_file_in, fd_file_out;

int process_input(char *args[], char input[], int *bg);

int main(void)
{
  char *token, input[MAX_LINE], copy[MAX_LINE], hist_buf[MAX_LINE],pipe_buf[MAX_LINE];
  char *args[MAX_LINE / 2 + 1];
  int should_run = 1, bg = 0, pipe_present = 0;
  int fd_pipe[2];
  pid_t pid;
  int i;

  while (should_run)
  {
    fd_file_in = STDIN_FILENO, fd_file_out = STDOUT_FILENO;
    bg = 0, pipe_present = 0;
    printf("osh>");
    fflush(stdout);

    /* read from stdin */
    fgets(input, MAX_LINE, stdin);
    input[strlen(input)-1] = '\0'; // remove carriage return 
    if(strlen(input) == 0)
      continue;

    /* history feature */
    if(strncmp(input,"!!",2) == 0){
      strcpy(input,hist_buf);
    }
    /* check for pipe */
    strcpy(copy, input);
    token = strtok(copy, "|"); // split 
    strcpy(input, token); // copy first command into input
    token = strtok(NULL, "|");
    if (token != NULL)
    {
      strcpy(pipe_buf, token);
      pipe_present = 1;
    }
    /* process input into arguments */
    process_input(args, input, &bg);


    /* check for exit command */
    if (strcmp(args[0], "exit") == 0)
    {
      should_run = 0;
      continue;
    }

    /* forking child and parent processes */
    pid = fork();
    if (pid < 0)
    {
      fprintf(stderr, "Fork error");
      return 1;
    }
    else if (pid == 0)
    {
      /* child process */
      /* perform I/O redirection */
      if (fd_file_in != STDIN_FILENO)
        dup2(fd_file_in, STDIN_FILENO);
      if (fd_file_out != STDOUT_FILENO)
        dup2(fd_file_out, STDOUT_FILENO);

      if (pipe_present)
      {
        if (pipe(fd_pipe) == -1)
        {
          fprintf(stderr, "pipe creation failed");
          return 1;
        }
        int pidc = fork();
        if (pidc < 0)
        {
          fprintf(stderr, "forking failed");
          return 1;
        }
        else if (pidc == 0)
        {
          close(fd_pipe[0]);
          /* process pipe_command */
          dup2(fd_pipe[1], STDOUT_FILENO);
          if (execvp(args[0], args) == -1) // try to execute args
          {
            fprintf(stderr, "operation failed");
            return 1;
          }
        }
        else
        {
          close(fd_pipe[1]);
          dup2(fd_pipe[0], STDIN_FILENO);
          wait(NULL);
          process_input(args, pipe_buf, &bg);
          if (execvp(args[0], args) == -1) // try to execute args
          {
            fprintf(stderr, "operation failed");
            return 1;
          }
        }
      }
      else if (execvp(args[0], args) == -1) // try to execute args
      {
        return 1;
      }
    }
    else
    {
      /* parent process */

      /* close any opened files */
      if (fd_file_in != STDIN_FILENO)
        close(fd_file_in);
      if (fd_file_out != STDOUT_FILENO)
        close(fd_file_out);

      if (!bg)
        wait(NULL);
      strcpy(hist_buf,input);
    }
  }
  return 0;
}

int process_input(char *args[], char input[], int *bg)
{
  char *token, copy[strlen(input)];
  int i, j;
  /* tokenize  */
  i = 0;
  strcpy(copy, input);
  token = strtok(copy, " ");
  while (token != NULL)
  {
    /* why strdup see :
    https://stackoverflow.com/questions/8407589/array-changing-values-after-fork
    since `token` is present only in the stack during function call the values pointed
    by `token` are all overwritten once the fucntion returns. strdup dynamically allocates
    memory outside the stack using malloc and hence the data is not overwritten.
    of course a global variable can also be used alternatively but I left this 
    useful insight as it is */
    args[i++] = strdup(token);
    token = strtok(NULL, " ");
  }
  args[i] = NULL;

  /* check if command meant to run in background */
  if (*args[i - 1] == '&')
  {
    *bg = 1;
    args[--i] = NULL;
  }
  else
    *bg = 0;

  /* check for redirect operators and set redirection files */
  i = 0, j = 0;
  while (args[i] != NULL)
  {
    if (*args[i] == '>')
    {
      fd_file_out = open(args[++i], O_CREAT | O_WRONLY, 0644);
      i++;
    }
    else if (*args[i] == '<')
    {
      fd_file_in = open(args[++i], O_RDONLY);
      i++;
    }
    else
    {
      /* overwrite the redirection operators and operands */
      args[j++] = args[i++];
    }
  }
  args[j] = NULL; //NULL terminate args
  return 1;
}