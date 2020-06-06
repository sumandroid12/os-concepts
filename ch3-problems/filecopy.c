#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#define WRITE_END 1
#define READ_END 0

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage: cp file1 file2");
        return 1;
    }
    char *src_name, *dst_name, buffer[BUFSIZ];
    src_name = argv[1];
    dst_name = argv[2];
    int fd_src, fd_dst, ret_in, ret_out; // file descriptors
    fd_src = open(argv[1], O_RDONLY);
    if (fd_src == -1)
    {
        perror("open");
        return 2;
    }
    pid_t pid;
    int fd[2];
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe Creation failed");
        return 2;
    }
    pid = fork();
    if (pid < 0)
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 4;
    }
    else if (pid > 0)
    { /* parent process */
        close(fd[READ_END]);
        while ((ret_in = read(fd_src, &buffer, BUFSIZ)) > 0)
        {
            ret_out = write(fd[WRITE_END], &buffer, (ssize_t)ret_in);
            if (ret_in != ret_out)
            {
                perror("File error occured");
                return 3;
            }
        }
        close(fd[WRITE_END]);
        wait(NULL);
    }
    else //child process
    {
        close(fd[WRITE_END]);
        fd_dst = open(argv[2], O_CREAT | O_WRONLY,0644);
        if (fd_dst == -1)
        {
            perror("open");
            return 2;
        }
        while ((ret_in = read(fd[READ_END], &buffer, BUFSIZ)) > 0)
        {
            ret_out = write(fd_dst, &buffer, (ssize_t)ret_in);
            if (ret_in != ret_out)
            {
                perror("File error occured");
                return 5;
            }
        }
        close(fd[READ_END]);
    }
    close(fd_src);
    close(fd_dst);
    return 0;
}
