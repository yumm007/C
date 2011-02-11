#include <stdio.h>

int
main()
{
	char *linebufp = NULL;
	size_t linebufsize = 0;

	getline(&linebufp, &linebufsize, stdin);

	printf("Content-type: text/html\r\n\r\n");

	printf("<h1><center>%s</center></h1>", linebufp);

	free(linebufp);

	return 0;
}

