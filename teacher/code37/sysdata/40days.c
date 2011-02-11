#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define BUFSIZE 1024

int
main()
{
	time_t now;
	struct tm *broken_stamp;
	char linebuf[BUFSIZE];

	now = time(NULL);

	broken_stamp = localtime(&now);

	strftime(linebuf, BUFSIZE, "Now:\t%Y-%m-%d", broken_stamp);
	puts(linebuf);

	broken_stamp->tm_mday += 40;

	(void)mktime(broken_stamp);

	strftime(linebuf, BUFSIZE, "Later:\t%Y-%m-%d", broken_stamp);
	puts(linebuf);

	return 0;
}

