#include "s3c2440.h"
#include "adc_ts.h"

void (*printf)(const char *, ...) = (void *)0x33f9291c;

void touch(void)
{
	struct adc *const adc = get_base_adc();

	if(adc->ADCUPDN & 0x1)
	{
		printf("Stylus down.....\n");
		adc->ADCUPDN &= ~1;
		adc->ADCTSC = AUTO_XY;
		adc->ADCCON |= 1;
	}
	if(adc->ADCUPDN & 0x2)
	{
		printf("Stylus up.....\n");
		adc->ADCUPDN &= ~2;
		adc->ADCTSC = WAIT_DOWN;
	}
}

void adc(void)
{
	struct adc *const adc = get_base_adc();

	printf("X = %d,  Y = %d\n", adc->ADCDAT0 & 0x3ff, adc->ADCDAT1 & 0x3ff);

	adc->ADCTSC = WAIT_UP;
}

void do_irq(void)
{
	struct interrupt *const irq = get_base_interrupt();

	if(irq->INTOFFSET == 31)
	{
		if(irq->SUBSRCPND & 0x200)
		{
			touch();
			irq->SUBSRCPND = 0x200;
		}
		
		if(irq->SUBSRCPND & 0x400)
		{
			adc();
			irq->SUBSRCPND = 0x400;
		}
	}

	irq->SRCPND = irq->INTPND;
	irq->INTPND = irq->INTPND;
}




