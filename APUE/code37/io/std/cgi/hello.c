#include <stdio.h>

int
main()
{
	/* MIME */
	printf("Content-type: text/html\r\n\r\n");

	puts("<h1><center>Hello!</center></h1>");

	return 0;
}

