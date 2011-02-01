#include <common.h>
#include <stdarg.h>
#include <ctype.h>

extern int serial_getc(void);
extern int serial_putc(int);

int getchar(void)
{
	return serial_getc();
}

int putchar(const char c)
{
	return serial_putc(c);
}

int gets(char *s)
{
	int c;
	int n = 0;
	while((c = getchar()) != '\n'){
		if(c == -1){
			*s = 0;
			return -1;
		}
		s[n++] = c;
	}
	s[n] = '\0';
	return n;
}

void puts(const char *s)
{
	while(*s)
		putchar(*s++);
	putchar('\n');
}

extern int vsprintf(char *buf, const char *fmt, va_list args);

void printf(const char *fmt, ...)
{
	va_list args;
	char printbuffer[1024];
	char *s = printbuffer;

	va_start(args, fmt);

	vsprintf(printbuffer, fmt, args);

	va_end(args);
	
	while(*s)
		putchar(*s++);
}

void sprintf(char *buf, const char *fmt, ...)
{
	va_list args;
	
	va_start(args, fmt);
	
	vsprintf(buf,fmt,args);
	
	va_end(args);
}

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (*cp == '0') {
		cp++;
		if ((*cp == 'x') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

long simple_strtol(const char *cp,char **endp,unsigned int base)
{
	if(*cp=='-')
		return -simple_strtoul(cp+1,endp,base);
	return simple_strtoul(cp,endp,base);
}

int ustrtoul(const char *cp, char **endp, unsigned int base)
{
	unsigned long result = simple_strtoul(cp, endp, base);
	switch (**endp) {
	case 'G' :
		result *= 1024;
		/* fall through */
	case 'M':
		result *= 1024;
		/* fall through */
	case 'K':
	case 'k':
		result *= 1024;
		if ((*endp)[1] == 'i') {
			if ((*endp)[2] == 'B')
				(*endp) += 3;
			else
				(*endp) += 2;
		}
	}
	return result;
}

void nsleep(unsigned int loops)
{
	__asm__ __volatile__ (
		"1:\n\t"
		"subs %0, %1, #1\n\t"
		"bne 1b"
		:"=r"(loops)
		:"0"(loops)
	);
}

void usleep(unsigned int loops)
{
	loops *= 100;
	__asm__ __volatile__ (
		"1:\n\t"
		"subs %0, %1, #1\n\t"
		"bne 1b"
		:"=r"(loops)
		:"0"(loops)
	);
}

void msleep(unsigned int loops)
{
	loops *= 100000;

	__asm__ __volatile__ (
		"1:\n\t"
		"subs %0, %1, #1\n\t"
		"bne 1b"
		:"=r"(loops)
		:"0"(loops)
	);
}

void sleep(unsigned int loops)
{
	msleep(loops * 100);
}

int atoi(const char *cp)
{
	return simple_strtol(cp, NULL, 10);
}

int atoh(const char *cp)
{
	return simple_strtol(cp, NULL, 16);
}


