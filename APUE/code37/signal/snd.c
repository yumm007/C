#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int
main(int argc, char **argv)
{
	union sigval val;

	if (argc<3) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	val.sival_int = atoi(argv[2]);
	return sigqueue(atoi(argv[1]), SIGUSR1, val);
}

