#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int
main(int argc, char **argv)
{
	if (argc<2) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	printf("Before: R=%d, E=%d\n", getuid(), geteuid());
	if (setuid(atoi(argv[1]))<0) {
		perror("setiod()");
		exit(1);
	}
	printf("After: R=%d, E=%d\n", getuid(), geteuid());
	return 0;
}

