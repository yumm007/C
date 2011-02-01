#include <stdio.h>

void print_bit(int n) {
	if (n >1 )
		print_bit( n / 2);
	putchar( n % 2 + '0');
}


int main(void)
{
	double n = 1.0;
	int i;
	int *p = (int *)&n;
	print_bit(*p);
	printf("\t");
	print_bit(*(p+1));
	printf("\n");

	return 0;
}
