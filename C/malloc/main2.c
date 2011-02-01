#include <stdio.h>
#include <stdlib.h>
#include "mem2.h"

int main(void)
{
	char add[100];
	MEM *mem;
	int size;

	if ((mem = init_mem((void *)(add + 1), 60)) == NULL) {
		printf("init mem faild\n");
		return -1;
	}

	printf("add[] = %p, mem start: %p, size = %u\n", add, mem, *((size_t *)mem));

	size = 15;
	printf("resize of %d, %d\n", size, align_size(size));
	
	return 0;
}


