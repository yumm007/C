#include <stdio.h>

int
main()
{
	FILE *fp;

	fp = tmpfile();

	while (1) pause();

	return 0;
}

