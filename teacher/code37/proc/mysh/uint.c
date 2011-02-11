#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void
int_handler(int s)
{
	putchar('O');
	fflush(stdout);
}

int
main()
{
	int i;

	signal(SIGINT, int_handler);

	for (i=0;;++i) {
		putchar('x');
		fflush(stdout);
		sleep(2);
	}
}

