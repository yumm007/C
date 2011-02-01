#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *get_mem(void)
{
#if 0
	char buf[10];
#else
	char *buf;	
	buf = malloc(10);
#endif
	memcpy(buf, "hello", 6);	
	return buf;
}

int main(void)
{
	char *p;
	p = get_mem();		
	printf("[%s]\n", p);
	return 0;
}
