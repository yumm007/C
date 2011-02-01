#include <stdio.h>

void itoa(int n) {
	if (n > 9)
		itoa(n / 10);
	printf ("%d", n % 10);
}

