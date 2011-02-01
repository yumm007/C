#include "s3c2440.h"


void (*printf)(const char *, ...) = (void *)0x33f9291c;

static char *dst = (void *)0x30100000;
static char *src = (void *)(0x31000000 + 1024 * 1024);

void do_irq(void)
{
	int i;
	struct interrupt *const irq = get_base_interrupt();

	if(irq->INTOFFSET == 17)
	{
		for(i = 0; i < 1024 * 1024; i++)
			dst[i] = src[i];
		src += 1024 * 1024;

//		printf("src = %080x\n", src);
	}

	irq->SRCPND = irq->INTPND;
	irq->INTPND = irq->INTPND;
}




