#include <stdio.h>
#include <time.h>

int
main()
{
	time_t stamp;

	/* MIME */
	printf("Content-type: text/html\r\n\r\n");

	stamp = time(NULL);

	printf("<h1><center>%d</center></h1>", stamp);

	return 0;
}

