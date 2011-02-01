#include <stdio.h>

int main(void)
{
	union foo {
		int p;
		char s;
	} u;

	u.p = 65;

	printf("%d\n", u.p);
	printf("%c\n", u.s);

	return 0;
}
