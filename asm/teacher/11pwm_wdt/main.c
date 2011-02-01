#include "s3c2440.h"
#include "common.h"

int main(void)
{
	struct interrupt *const irq = get_base_interrupt();
	struct watchdog *const wdt = get_base_watchdog();
	struct timers *pwm = get_base_timers();
	
	wdt->WTDAT = 62500;	
	wdt->WTCNT = 62500;
	wdt->WTCON = 1 | 49 << 8;

/*****************************************/	

	pwm->TCFG0 = 49 << 8;
	pwm->TCFG1 = 3 << 16;

	pwm->TCNTB4 = 50000;

	set0(irq->INTMSK, 14);

/********************************************/

	pwm->TCON = 5 << 20;

	wdt->WTCON |= 1 << 5;
	
	__asm__ __volatile__ (
		"mrs	r0, cpsr\n\t"
		"bic	r0, r0, #0x80\n\t"
		"msr	cpsr, r0\n\t"
		:::"r0"
	);

	return 0;
}

