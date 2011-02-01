#include <stdio.h>

int fact(int n) {
	return ((n == 1) ? 1 : n * fact(n - 1));
}

int main(void)
{
	printf("6!=%d\n",fact(6));

	return 0;
}
