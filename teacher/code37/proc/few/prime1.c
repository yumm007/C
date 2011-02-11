#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define LEFT 30000000
#define RIGHT 30000200

int
main()
{
	int i, j, mark;
	pid_t pid;
	int status;

	for (i=LEFT;i<=RIGHT;++i) {
		fflush(NULL);
		pid = fork();
		if (pid<0) {
			perror("fork()");
			exit(1);
		}
		if (pid==0) {
			mark = 1;
			for (j=2;j<i/2;++j) {
				if (i%j==0) {
					mark=0;
					break;
				}
			}
			if (mark==1) {
				printf("%d is a prime.\n", i);
			}
			exit(0);  /* !!!!!!!!! */
		}
	}

	for (i=LEFT;i<=RIGHT;++i) {
		pid = wait(&status);
		if (WIFSTOPPED(status) || WIFCONTINUED(status)) {
			--i;
		} else {
			printf("%d terminated.\n", pid);
		}
	}

	return 0;
}

