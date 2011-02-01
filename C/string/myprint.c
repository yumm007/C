#include <stdio.h>

void __print_int(const unsigned int n) {
	if (n >= 10) 
		__print_int(n / 10);
	putchar(n % 10 + '0');
}

void print_int(int n) {

	if (n < 0){
		putchar('-');
		__print_int(n / -10);
		putchar(-(n % -10) + '0');
	} else
		__print_int(n);
}

void print_hex(unsigned long int n) {
	int i;

	if (n >= 16) 
		print_hex(n / 16);
	
	i = n % 16;
	if (i >= 10)
		putchar('A'+ i - 10);
	else
		putchar(i + '0');
}

void print_point(unsigned long int point) {
	if (point == 0){
		putchar('(');
		putchar('n');
		putchar('i');
		putchar('l');
		putchar(')');
		return;
	}
	putchar('0');
	putchar('x');
	print_hex(point);
}

void print_str(const char *s) {
	int i = 0;
	while (s[i] != '\0')
		putchar(s[i++]);
}

void print_flo(double flo) {
	int n = 0;
	n = flo;
	print_int(n);
	putchar('.');
	n = (flo - n) * 1000000;
	if (n < 0)
		n = -n;
	print_int(n);
}

void print_double(double n){
     unsigned int num, index;
  
     int *p = (int *)&n;
     index = num = *(p+1);
     num = (~(~0<<20)) & num | 1<<20;
     index = ((index >> 20) & ~(~0<<20)) - 1023;
     num = (num >> (20 -index)) & ~(~0<<(index+1));

     putchar((*(p+1) < 0)? '-':' ');
		__print_int(num);
}

int myprintf(const char *format, ...) {

	int c, flag = 0;
	char *parg = (char *)(&format + 1);

	while ( *format != '\0' ) {
		switch (c = *format++) {
			case '%':
				switch (c = *format++) {
					case 'd':
						print_int(*(int *)parg);
						parg += sizeof(int);
						break;
					case 's':
						print_str(*(char **)parg);
						parg += sizeof (char **);
						break;
					case 'c':
						putchar(*(char *)parg);
						parg += sizeof(int);
						break;
					case 'f':
						print_flo(*(double *)parg);
						parg += sizeof(double);
						break;
					case 'x':
						print_hex(*(int *)parg);
						parg += sizeof(int);
						break;
					case 'p':
						print_point(*(unsigned long int*)parg);
						parg += sizeof(unsigned long int *);
						break;
					default:
						putchar(c);
						break;
				}
				break;
			default:
				putchar(c);
				break;
		}
	}
}


int main(void)
{
	int n = 255;
	myprintf("%%,%aa\t%d,%f,%s%d%c\n",-345133,-123.123,"this is test line",89,'d');
	myprintf("%x,%p,%s,%p\n",n,&n,"end of line",NULL);
	
	print_double(-123.57);
	printf("\n");
	
	return 0;
}
