#include "s3c2440.h"

void (*printf)(const char *, ...) = (void *)0x33f9291c;

void do_irq(void)
{
	struct interrupt *const irq = get_base_interrupt();

	struct watchdog *wdt = get_base_watchdog();

	if(irq->INTOFFSET == 14)
	{
		printf("feed dog.......\n");
		
		wdt->WTCNT = wdt->WTDAT;
	}
	
	irq->SRCPND = irq->INTPND;
	irq->INTPND = irq->INTPND;
}


