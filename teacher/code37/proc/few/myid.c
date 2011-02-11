#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int
main()
{
	printf("R=%d, E=%d\n", getuid(), geteuid());
	return 0;
}

