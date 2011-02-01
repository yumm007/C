#include "s3c2440.h"

void (*printf)(const char *, ...) = (void *)0x33f9291c;

void do_irq(void)
{
	struct interrupt *const irq = get_base_interrupt();

	if((irq->INTOFFSET == 9) && (irq->SUBSRCPND & 0x2000))
	{
		printf("wdt.......\n");
		irq->SUBSRCPND = 0x2000;
	}
	
	irq->SRCPND = irq->INTPND;
	irq->INTPND = irq->INTPND;
}












