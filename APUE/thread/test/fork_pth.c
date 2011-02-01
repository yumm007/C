#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define	PID_MAX		100000

int main(void)
{
	pid_t pid;
	int i;

	for (i = 0; i < PID_MAX; i++) {
		if ((pid = fork()) < 0){
			fprintf(stderr, "fork(): %s\n", strerror(errno));
			return -1;
		} else if (pid == 0) {
			i = 4;
			exit(0);
		} else
			wait(NULL);
	}

	return 0;
}
