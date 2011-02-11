#include <stdio.h>
#include <time.h>

int
main()
{
	time_t end;

	end = time(NULL)+5;

	while (time(NULL)<end) {
		putchar('x');
	}

	return 0;
}

