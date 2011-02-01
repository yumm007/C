
#include <common.h>
#include <stdarg.h>
#include <ctype.h>

extern int console_getc(void);
extern int console_putc(int);

int str_cmp(const char *a1, const char *b1) {
   char *a = (char *)a1, *b = (char *)b1; 

   while (*a == *b && *a != '\0' && *b != '\0') {
      a++;
      b++;
   }   

   return *a - *b; 
}

void mem_set(char *buf, int val, int len) {
	int i;
	for (i = 0; i < len; i++)
      buf[i] = val;
}

void mem_cpy(char *dest, char *sour, int len) {
	int i;
	for (i = 0; i < len; i++)
		dest[i] = sour[i];
}

static unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
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
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp) \
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

static long simple_strtol(const char *cp,char **endp,unsigned int base)
{
	if(*cp=='-')
		return -simple_strtoul(cp+1,endp,base);
	return simple_strtoul(cp,endp,base);
}

int atoi(const char *cp)
{
	return simple_strtol(cp, NULL, 10);
}

int atoh(const char *cp)
{
	return simple_strtol(cp, NULL, 16);
}

