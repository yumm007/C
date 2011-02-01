#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	char *p, *p1;
	p = malloc(10);	
	if(NULL == p)
	{
		return -1;	
	}
	memcpy(p, "hello", 6);
	printf("[%s]\n", p);

	p1 = malloc(10);	
	if(NULL == p1)
	{
		free(p);
		return -1;	
	}
	memcpy(p1, "hoooo", 6);
	printf("[%s]\n", p1);

	free(p);
	free(p1);
	return 0;
}
