#include "s3c2440.h"
#include "common.h"
#include "adc_ts.h"

int main(void)
{
	struct interrupt *const irq = get_base_interrupt();
	struct adc *const adc = get_base_adc();


	adc->ADCCON = (1 << 14) | (49 << 6);
	adc->ADCTSC = WAIT_DOWN;

	set0(irq->INTMSK, 31);
	clrbit(irq->INTSUBMSK, 0x3, 9);

	__asm__ __volatile__ (
		"mrs	r0, cpsr\n\t"
		"bic	r0, r0, #0x80\n\t"
		"msr	cpsr, r0\n\t"
		:::"r0"
	);

	return 0;
}


