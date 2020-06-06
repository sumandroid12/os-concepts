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
	const int SIZE=4096;
	const char *name="timestamp";
	struct timeval start,*ptr;
	pid_t pid;
	int fd;
	fd = shm_open(name,O_CREAT | O_RDWR,0666); 
	ftruncate(fd,SIZE);
	/* fork a child process */
	pid = fork();
	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid == 0) { /* child process */
		struct timeval current;
		gettimeofday(&current,NULL);
		ptr = (struct timeval *)
			mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
		*ptr = current;
		execlp(argv[1], argv[1], NULL);
	}
	else { /* parent process */
		wait(NULL);
		ptr = (struct timeval *)
			mmap(0,SIZE, PROT_READ , MAP_SHARED, fd, 0);
		start = *ptr;
		struct timeval end;
		gettimeofday(&end,NULL);
		printf("time %lds %ldus \n",
			(long) (end.tv_sec - start.tv_sec),
			(long) (end.tv_usec - start.tv_usec));
		shm_unlink(name);
	}
	return 0;
}
