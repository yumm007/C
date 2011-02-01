#include <stdio.h>

int main(void)
{
	char *linebufp = NULL;
	size_t linebufsize = 0;

	getline(&linebufp, &linebufsize, stdin);

	printf("Content-type: text/html\r\n\r\n");

	printf("<h1><center>Welceme!</center></h1>", linebufp);
	printf("<form action=\"/cgi-bin/login.cgi\" method=\"POST\">");
	printf("shell：<input type=\"text\" name=\"shell\"><br>");
	printf("<input type=\"submit\">");
	printf("</form>");

	free(linebufp);

	return 0;
}

