#include "s3c2440.h"
#include "common.h"

int main(void)
{
	struct interrupt *const irq = get_base_interrupt();
	
	struct watchdog *const wdt = get_base_watchdog();

	
	wdt->WTDAT = 30000;	
	wdt->WTCNT = 30000;
	wdt->WTCON = 0x8024;

#if 0

	while(1)
	{
		if(wdt->WTCNT < 100)
			wdt->WTCNT = 50000;
	}
#endif

	set0(irq->INTMSK, 9);

	set0(irq->INTSUBMSK, 13);

#if 1
	__asm__ __volatile__ (
		"mrs	r0, cpsr\n\t"
		"bic	r0, r0, #0x80\n\t"
		"msr	cpsr, r0\n\t"
		:::"r0"
	);
#endif
	return 0;
}

