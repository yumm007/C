#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LEFT 30000000
#define RIGHT 30000200

static void
chld_handler(int s)
{
	while (waitpid(-1, NULL, WNOHANG)>0);
/*
	pid_t pid;
	int status;
	pid = wait(&status);
	if (!WIFSTOPPED(status) && !WIFCONTINUED(status)) {
		printf("%d terminated.\n", pid);
	}
*/
}

int
main()
{
	int i, j, mark;
	pid_t pid;
	struct sigaction sa;

	sa.sa_handler = chld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_NOCLDSTOP;

	sigaction(SIGCHLD, &sa, NULL);

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

	while (1) {
		putchar('x');
		fflush(stdout);
		sleep(1);
	}

	return 0;
}

