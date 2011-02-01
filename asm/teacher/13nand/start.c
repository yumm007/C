#include "ctype.h"

void (*printf)(const char *, ...) = (void *)0x33f90e04;

int atoi(const char *cp);
int strcmp(const char * cs,const char * ct);

void _start(int argc, char *argv[])
{
	nand_init();

	if((argc == 5) && (strcmp(argv[1], "read") == 0))
	{
		nand_read(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
		printf("read ok....\n");
	}
	else if((argc == 5) && (strcmp(argv[1], "write") == 0))
	{
		nand_write(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
		printf("write ok....\n");
	}
	else if((argc == 4) && (strcmp(argv[1], "erase") == 0))
	{
		nand_erase(atoi(argv[2]), atoi(argv[3]));
		printf("erase ok....\n");
	}
	else
	{
		printf("args error....\n");
	}
}

static unsigned int simple_strtoul(const char *cp)
{
	unsigned int result = 0, value, base = 0;

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
	
	return result;
}

int atoi(const char *cp)
{
	if(*cp=='-')
		return -simple_strtoul(cp + 1);
	
	return simple_strtoul(cp);
}

int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}
