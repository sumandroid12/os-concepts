#include<sys/types.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/mman.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc, char *argv[])
{ 
	pid_t pid, pid1;
	/* fork a child process */
	pid = fork();
	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid == 0) { /* child process */
		execlp(argv[1],argv[1],NULL);
	}
	else { /* parent process */
		wait(NULL);
	}
	return 0;
}
