#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


/* ./mysu UID command arg ... */
int
main(int argc, char **argv)
{
	pid_t pid;

	if (argc<3) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	if (setuid(atoi(argv[1]))<0) {
		perror("setiod()");
		exit(1);
	}

	pid = fork();
	if (pid<0) {
		perror("fork()");
		exit(1);
	}
	if (pid==0) {
		execv(argv[2], &argv[2]);
		perror("execvp()");
		exit(1);
	}
	wait(NULL);

	return 0;
}

