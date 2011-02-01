#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//fault
#if 0
void get_mem(char *buf)
{
	buf = malloc(10);
	memcpy(buf, "hello", 6);	
}

int main(void)
{
	char *p;
	get_mem(p);		
	printf("[%s]\n", p);
	return 0;
}

#else
//right
void get_mem(char **buf)
{
	*buf = (char *)malloc(10);
	memcpy(*buf, "hello", 6);	
}

int main(void)
{
	char *p;
	get_mem(&p);		
	printf("[%s]\n", p);
	return 0;
}

#endif
