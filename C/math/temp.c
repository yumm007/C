#include <stdio.h>

int itoa(int n) {
	if (n == 1)
		return 1;
	else
		return( n + itoa(n - 1));
}

