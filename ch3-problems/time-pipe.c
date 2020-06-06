#include<sys/types.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/mman.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

#define WRITE_END 1
#define READ_END 0


int main(int argc, char *argv[])
{ 
	int fd[2];
	pid_t pid;
	if(pipe(fd) == -1){
		fprintf(stderr, "Pipe Creation failed");
		return 1;
	}

	pid = fork();

	if(pid < 0){
		fprintf(stderr, "Forking failed");
		return 1;
	}
	else if(pid == 0) // child process
	{
		printf("size %d", sizeof(struct timeval *));
		close(fd[READ_END]);
		struct timeval start;
		gettimeofday(&start,NULL);
		write(fd[WRITE_END], &start, sizeof(struct timeval));
		close(fd[WRITE_END]);
		execlp(argv[1], argv[1], NULL);
	}
	else{
		close(fd[WRITE_END]);
		wait(NULL);
		struct timeval end,start;
		gettimeofday(&end,NULL);
		read(fd[READ_END],&start,sizeof(struct timeval));
		close(fd[READ_END]);
		
		// printf("time %lds %ldus \n",
		// 	(long) (end.tv_sec),
		// 	(long) (end.tv_usec));
		// printf("time %lds %ldus \n",
		// 	(long) (start.tv_sec),
		// 	(long) (start.tv_usec));
		printf("time %lds %ldus \n",
			(long) (end.tv_sec - start.tv_sec),
			(long) (end.tv_usec - start.tv_usec));
	}
}
