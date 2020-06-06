#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include<sys/shm.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
int main(int argc, char *argv[])
{
    int input = atoi(argv[1]);
    if (argc == 1 || input <= 0)
    {
        printf("ENTER POSITIVE NUMBERS ONLY");
        return 1;
    }
    pid_t pid;
    const int SIZE = 4096;
    const char *shm_name = "collatz";
    int fd,*data,*head;
    fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);
    pid = fork();
    if (pid < 0)
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0)
    { /* child process */ 
        int count = 0;
        head = (int *) 
            mmap(0,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        data = head;
        data++;
        while (input != 1)
        {
            if (input % 2 == 0)
            {
                input /= 2;
            }
            else
            {
                input = input * 3 + 1;
            }
            *data = input; 
            data++; count++;
        } 
        *head=count;
    }
    else
    { /* parent process */
        wait(NULL);
        head = (int *) mmap(0,SIZE,PROT_READ,MAP_SHARED,fd,0);
        data = head;
        data++;
        printf("%d ",input);
        for(int i=*head;i>0;i--){ 
            printf("%d ", *data);
            data++;
        }
        shm_unlink(shm_name);
    }
    printf("\n");
    return 1;
}