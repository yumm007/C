#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* date +%s */

int
main()
{
	pid_t pid;

	puts("Before few.");
	fflush(NULL);
	pid=fork();
	if (pid<0) {
		perror("fork()");
		exit(1);
	}
	if (pid==0) {
		execl("/bin/date", "date", "+%s", NULL);
		perror("execl()");
		return 1;
	}
	wait(NULL);
	puts("After few.");
	return 0;
}

