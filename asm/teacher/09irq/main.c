#include "s3c2440.h"
#include "common.h"

int main(void)
{
	struct interrupt *const irq = get_base_interrupt();
	struct gpio *const gpio = get_base_gpio();
	
	setval(gpio->GPFCON, 0xaa, 8, 0);
	setval(gpio->GPGCON, 0xa, 4, 0);

	clrbit(gpio->EINTMASK, 0x3, 8);

	clrbit(irq->INTMSK, 0x2f, 0);

//	setval(gpio->EXTINT0, 0x1, 3, 4);

	__asm__ __volatile__ (
		"mrs	r0, cpsr\n\t"
		"bic	r0, r0, #0x80\n\t"
		"msr	cpsr, r0\n\t"
		:::"r0"
	);

	return 0;
}



