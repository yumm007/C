#include "s3c2440.h"
#include "common.h"
#include "adc.h"

#define LCD_WIDTH		480
#define LCD_HEIGHT	272
#define BUF_OFFSET	(*((int volatile *)0x30fffffc))
#define BUF_BASE		((unsigned int)0x31000000)
#define PIC_SIZE		261120

void enable_intr(void) {
	struct interrupt *intr = get_base_interrupt();
	struct adc *adc = get_base_adc();

	/*打开信号处理*/
	setval(intr->INTMSK, 0x0, 1, 31);
	setval(intr->INTSUBMSK, 0x0, 2, 9);

	/*设置ADC控制器*/
	adc->ADCCON = (1 << 14) | (49 << 6);
	adc->ADCTSC = 0xd3;
	adc->ADCDLY = 320000;

	__asm__ __volatile__ (
		"mrs r0, cpsr\n\t"
		"bic r0, r0, #0x80\n\t"
		"msr cpsr, r0"
		:::"r0"
	);
	
}

void change_addr(void) {
	struct lcd *const lcd = get_base_lcd();
	unsigned int volatile buf;

	buf = BUF_BASE + *((int *)0x30fffffc) * PIC_SIZE;

	lcd->LCDSADDR1 = buf >> 1;
	lcd->LCDSADDR2 = (buf + LCD_WIDTH * LCD_HEIGHT * 2) >> 1;
	lcd->LCDSADDR3 = LCD_WIDTH;
}

int main(void) {
	struct lcd *const lcd = get_base_lcd();
	struct gpio *const gpio = get_base_gpio();

	BUF_OFFSET = 0;

	/* GPIO 设置*/
	setval(gpio->GPBCON, 0x01, 2, 0); /*lcd 灯光控制*/
	gpio->GPCCON = 0xaaaaaaaa;
	gpio->GPCUP =0xffff;
	gpio->GPDCON = 0xaaaaaaaa;
	gpio->GPDUP =0xffff;

	/*lcd 设置*/
	setval(lcd->LCDCON1,0x0,1,0);
	setval(lcd->LCDCON1,12,4,1);
	setval(lcd->LCDCON1,0x3,2,5);
	setval(lcd->LCDCON1,0x4,10,8);

	setval(lcd->LCDCON2, 0xa, 6, 0);
	setval(lcd->LCDCON2, 0x2, 8, 6);
	setval(lcd->LCDCON2, 271, 10, 14);
	setval(lcd->LCDCON2, 0x2, 8, 24);

	setval(lcd->LCDCON3, 0x2, 8, 0);
	setval(lcd->LCDCON3, 479, 11, 8);
	setval(lcd->LCDCON3, 0x2, 7, 19);

	setval(lcd->LCDCON4, 41, 8, 0);

	setval(lcd->LCDCON5, 0x1, 1, 0);
	setval(lcd->LCDCON5, 0x3, 2, 8);
	setval(lcd->LCDCON5, 0x1, 1, 11);

	change_addr();

	/*启用*/
	gpio->GPBDAT |= 1; /*亮灯*/
	setval(lcd->LCDCON5, 0x1, 1, 3);
	setval(lcd->LCDCON1, 0x1, 1, 0);

	enable_intr();

	return 0;
}
