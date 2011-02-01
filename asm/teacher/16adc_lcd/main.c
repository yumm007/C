#include "lcd.h"
#include "s3c2440.h"
#include "common.h"
#include "adc_ts.h"


#define BUF ((unsigned int)0x31000000)

void lcd_init(void)
{
	struct gpio *const gpio = get_base_gpio();
	struct lcd *const lcd = get_base_lcd();
	

	setval(gpio->GPBCON, 0x1, 2, 0);

	gpio->GPCCON = 0xaaaaaaaa;
	gpio->GPCUP = 0xffff;

	gpio->GPDCON = 0xaaaaaaaa;
	gpio->GPDUP = 0xffff;

/************************************************/

	lcd->LCDCON1 = BPPMODE(0xc) | PNRMODE(0x3) | CLKVAL(0x4);

	lcd->LCDCON2 = VSPW(10) | VFPD(2) | LINEVAL(271) | VBPD(2);
	lcd->LCDCON3 = HFPD(2) | HOZVAL(479) | HBPD(2);
	lcd->LCDCON4 = HSPW(41);
	lcd->LCDCON5 = FRM565(1) | HWSWP(1) | INVVLINE(1) | INVVFRAME(1);


	lcd->LCDSADDR1 = LCDBANK(BUF >> 22) | LCDBASEU((BUF >> 1) & 0x1fffff);
	lcd->LCDSADDR2 = LCDBASEL(((BUF + 480 * 272 * 2) >> 1) & 0x1fffff);
	lcd->LCDSADDR3 = 480;

/**********************************************/
	
	gpio->GPBDAT |= 1;
	lcd->LCDCON5 |= PWREN(1);
	lcd->LCDCON1 |= ENVID(1);
}

int main(void)
{
	struct interrupt *const irq = get_base_interrupt();
	struct adc *const adc = get_base_adc();

	lcd_init();

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


