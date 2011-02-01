#include "lcd.h"
#include "adc_ts.h"
#include "s3c2440.h"

void (*printf)(const char *, ...) = (void *)0x33f9291c;

static int flag = 3;

void down(void)
{
	unsigned int buf;

	struct lcd *const lcd = get_base_lcd();

	switch(flag)
	{
		case 1:
			buf = 0x31000000;
			flag = 2;
			break;
		case 2:
			buf = 0x32000000;
			flag = 3;
			break;
		case 3:
		default:
			buf = 0x33000000;
			flag = 1;
			break;
	}

	lcd->LCDSADDR1 = LCDBANK(buf >> 22) | LCDBASEU((buf >> 1) & 0x1fffff);
	lcd->LCDSADDR2 = LCDBASEL(((buf + 480 * 272 * 2) >> 1) & 0x1fffff);	
	printf("down...\n");
}

void do_irq(void)
{	
	struct interrupt *const irq = get_base_interrupt();
	struct adc *const adc = get_base_adc();

	if((irq->INTOFFSET == 31) && (irq->SUBSRCPND & 0x200))
	{
		if(adc->ADCUPDN & 0x1)
		{
			down();
			adc->ADCTSC = WAIT_UP;
			adc->ADCUPDN &= ~0x1;
		}

		if(adc->ADCUPDN & 0x2)
		{
			adc->ADCTSC = WAIT_DOWN;
			adc->ADCUPDN &= ~0x2;
		}

		irq->SUBSRCPND = 0x200;
	}

	irq->SRCPND = irq->INTPND;
	irq->INTPND = irq->INTPND;
}


