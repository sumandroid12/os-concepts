#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int input = atoi(argv[1]);
	if (argc == 1 || input <= 0)
	{
		printf("ENTER POSITIVE NUMBERS ONLY");
		return 1;
	}
	pid = fork();
	if (pid < 0)
	{ /* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid == 0)
	{ /* child process */
		printf("%d ", input);
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
			printf("%d ", input);
		}
	}
	else
	{ /* parent process */
		wait(NULL);
	}
	printf("\n");
	return 1;
}