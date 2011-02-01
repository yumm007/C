#include "s3c2440.h"

void (*printf)(const char *, ...) = (void *)0x33f9291c;

void msleep(unsigned int loops)
{
	loops *= 100000;

	__asm__ __volatile__ (
		"1:\n\t"
		"subs %0, %1, #1\n\t"
		"bne 1b"
		:"=r"(loops)
		:"0"(loops)
	);
}

void do_irq(void)
{
	struct interrupt *const irq = get_base_interrupt();
	struct gpio *const gpio = get_base_gpio();

	switch(irq->INTOFFSET & 0x1f)
	{
		case 0:
			printf("sw1111111111111\n");
			break;
		case 1:
			printf("sw2222222222222\n");
			break;
		case 2:
			printf("sw3333333333333\n");
			break;
		case 3:
			printf("sw44444444444444\n");
			break;
		case 5:
			if(gpio->EINTPEND & 0x100)
			{
				printf("sw55555555555555\n");
				gpio->EINTPEND = 0x100;
			}
			if(gpio->EINTPEND & 0x200)
			{
				printf("sw66666666666666\n");
				gpio->EINTPEND = 0x200;
			}
			break;
	}

	msleep(20);

	irq->SRCPND = irq->INTPND;
	irq->INTPND = irq->INTPND;
}












