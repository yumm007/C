#include <stdio.h>

int op_sym = '%';

char *op_desc = "Modular";

int
op_func(int a, int b)
{
	return a%b;
}

