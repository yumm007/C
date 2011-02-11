#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int
main()
{
	pid_t pid;

	printf("%d: Before fork().\n", getpid());
	fflush(NULL);
	pid = fork();
	if (pid<0) {
		perror("fork()");
		exit(1);
	}
	if (pid==0) {
		printf("%d: Child is running.\n", getpid());
	} else {
		printf("%d: Parent is running.\n", getpid());
		wait(NULL);
	}
	return 0;
}

