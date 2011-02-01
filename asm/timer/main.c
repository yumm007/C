#include "s3c2440.h"
#include "common.h"

void (*printf)(const char *, ...) = (void *)0x33f9291c;

int main(void)
{
	struct gpio *const gp = get_base_gpio();
	struct interrupt *const intr = get_base_interrupt();
	struct timers *tim = get_base_timers();
	
	setval(gp->GPBCON, 0x2, 2, 4); /*打开GPIO里的2号计时器 */
	//setval(gp->GPBUP, 0x1, 1, 2); /* 关闭上拉电阻 */
	//setval(intr->INTMSK, 0x0, 1, 12); /* 关闭2号计时器信号屏蔽位 */
	set0(intr->INTMSK, 12);
	
	
	setval(tim->TCFG0, 0xFF, 8, 8); /* 一级分频器*/
	setval(tim->TCFG1, 0x3, 4, 8); /* 二级分频器*/
	tim->ch[2].TCNTB = 65530;
	tim->ch[2].TCMPB = 30000;
	
	setval(tim->TCON, 0x1, 1, 13); /* 开启2号计时器,并自动重载 */
	setval(tim->TCON, 0x9, 4, 12); /* 开启2号计时器,并自动重载 */

	/* 关闭cpsr中的信号屏蔽 */
	__asm__ __volatile (
		"mrs  r0, cpsr\n\t"
		"bic r0, r0, #0x80\n\r"
		"msr cpsr, r0\n\t"
		:::"r0"
	);
	
	return 0;
}

void do_irq(void) {
	struct interrupt *const intr = get_base_interrupt();

	if (intr->INTOFFSET == 12) {
		printf("get interrupt 12.\n");
	} else 
		printf("get unknow interrupt %d.\n", intr->INTOFFSET);

	/* 清除信号 */
	setval(intr->SRCPND, 0x1, 1, intr->INTOFFSET);
	setval(intr->INTPND, 0x1, 1, intr->INTOFFSET);
}
