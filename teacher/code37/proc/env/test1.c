#include <stdio.h>

static int
func_b(int n)
{
	return n+6;
}

static int
func_a(int a, int b)
{
	return func_b(a)+b;
}

int
main()
{
	printf("%d\n", func_a(5,6));
}
