#include "s3c2440.h"
#include "common.h"

void (*printf)(const char *, ...) = (void *)0x33f9291c;

void touch_screen(void) {
	struct interrupt *intr = get_base_interrupt();
	struct adc *adc = get_base_adc();

	if (intr->INTOFFSET != 31)
		return;

	if (intr->SUBSRCPND & 0x200) {
		adc->ADCUPDN &= ~1;
		adc->ADCTSC = 0x0c;
		adc->ADCCON |= 1;
		intr->SUBSRCPND = 0x200;
	} 
	if (intr->SUBSRCPND & 0x400) {
   printf("X = %d,  Y = %d\n", adc->ADCDAT0 & 0x3ff, adc->ADCDAT1 & 0x3ff);
		adc->ADCTSC = 0xd3;
		adc->ADCCON &= ~1;
		intr->SUBSRCPND = 0x400;
	}

	intr->SRCPND = intr->INTPND;
 	intr->INTPND = intr->INTPND;
}

