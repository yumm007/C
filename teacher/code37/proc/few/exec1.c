#include <stdio.h>
#include <unistd.h>

/* date +%s */

int
main()
{
	puts("Before exec.");
	fflush(NULL);
	execl("/bin/date", "date", "+%s", NULL);
	puts("After exec.");
	perror("execl()");
	return 0;
}

