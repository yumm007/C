#include <stdio.h>

#define var_start(ap, last_argv) \
	( (ap) = (char *)&(last_argv) + sizeof(last_argv) )

#define var_get(ap, type) \
	( (ap) += sizeof(type), *(type *)((ap) - sizeof(type)) )

#define var_end(ap)  

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
	putchar('0');
	putchar('x');
	print_hex(point);
}

void print_str(char *s) {
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

int myprintf(const char *format, ...) {

	int c, flag = 0;
	char *ap, *s;
	char *parg = (char *)(&format + 1);

	var_start(ap, format);

	while ( *format != '\0' ) {
		switch (c = *format++) {
			case '%':
				switch (c = *format++) {
					case 'd':
						print_int(var_get(ap, int));
						break;
					case 's':
						print_str((char *)var_get(ap, char **));
						break;
					case 'c':
						putchar(var_get(ap, int));
						break;
					case 'f':
						print_flo(var_get(ap, double));
						break;
					case 'x':
						print_hex(var_get(ap, int));
						break;
					case 'p':
						print_point(var_get(ap, unsigned long int));
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
	myprintf("%d,%s\t%c,%f,%x, %p\n",n,"test line",'A',-33.0,n, &n);
	printf("%p\n",&n);	
	return 0;
}
