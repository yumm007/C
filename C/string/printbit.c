#include <stdio.h>

/*
void printbit(unsigned int n){
	
	char num[]= "00000000 00000000 00000000 00000000";
	int i;

	for ( i = 34; i >= 0 && n > 0 ; i-- )
		if ( i != 8 && i != 17 && i != 26 ) {
			num[i] = n % 2 + '0';
			n = n >> 1;
		}
	
	printf("The bit is %s\n", num);
}
*/
/* 使用移位的方式打印二进制位 */
void printbit(unsigned int n) {
	int i;
	
	for (i = 31; i >= 0; i-- ) {
		if (i == 7 || i == 15 || i == 23)
			putchar(' ');
		putchar((n >> i & 1) + '0');
		n = ~(1 << i) & n;
	}
}

void __print_int(const unsigned int n) {
	if (n >= 10) 
		__print_int(n / 10);
	putchar(n % 10 + '0');
}

void __print_double(double n){

	int i;
	unsigned int *p =(unsigned int *)&n;
	for (i = sizeof(double) / sizeof(int); i>= 1; i--) {
		printbit(*(p+i-1));
		printf("\t");
	}
	printf("\n");
}

void print_double(double n){
	unsigned int num, index;
	
	int *p = (int *)&n;
	index = num = *(p+1);
	num = (~(~0<<20)) & num | 1<<20;
	printbit(index);
	printf("\n");
	index = ((index >> 20) & ~(~0<<20)) - 1023; 
	num = (num >> (20 -index)) & ~(~0<<(index+1));
#if 1
	printbit(index);
	printf("\n");
	printbit(num);
	printf("\nindex = %d, num = %c%d \n", index, (*(p+1) < 0)? '-':' ',num);
	__print_double(*(double *)p);
#else
	if (*(p+1) < 0)
		putchar('-');
	__print_int(num);
#endif
}

int main(void)
{
	double n = -11245.5;
	print_double(n);
	printf("\n");

	return 0;
}
