#include "s3c2440.h"
#include "common.h"
#include "adc.h"

#define BUF_OFFSET	(*((int volatile *)0x30fffffc))

extern void change_addr(void);

void touch_screen(void) {
	struct interrupt *intr = get_base_interrupt();
	struct adc *adc = get_base_adc();
	int n;

	if (intr->INTOFFSET != 31)
		return;

	if (intr->SUBSRCPND & 0x200) {
		setval(intr->SUBSRCPND, 1, 1, 9);
		adc->ADCTSC = 0x0c;
		adc->ADCCON |= 1;
	} 

	if (intr->SUBSRCPND & 0x400) {
		setval(intr->SUBSRCPND, 1, 1, 10);
		adc->ADCTSC = 0xd3;

		if ((adc->ADCDAT0 & 0x3ff) < 200) {
			n = BUF_OFFSET - 1;
			if (n < 0)
				n = 64;
		}else {
			n = BUF_OFFSET + 1;
			if (n == 65)
				n = 0;
		}
		BUF_OFFSET = n;
		change_addr();
	
	}

	/*清除信号*/
	intr->SRCPND = intr->INTPND;
	intr->INTPND = intr->INTPND;
}
