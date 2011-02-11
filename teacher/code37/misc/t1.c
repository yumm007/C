#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *p;

	p = malloc(sizeof(int));

	*p=7;

	return 0;
}

