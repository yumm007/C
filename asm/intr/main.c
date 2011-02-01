#include "s3c2440.h"
#include "common.h"

void (*printf)(const char *, ...) = (void *)0x33f9291c;

int main(void)
{
	struct gpio *const gp = get_base_gpio();
	struct interrupt *const intr = get_base_interrupt();
	
	setval(gp->GPFCON, 0xaa, 8, 0); /*打开GPIO里的0号1号中断*/
	setval(gp->GPFUP, 0xf, 4, 0); /* 关闭上拉电阻*/
	setval(gp->EXTINT0,0x4, 3, 12); /* 3号中断改为上升沿触发*/

	setval(intr->INTMSK, 0x0, 4, 0); /* 关闭信号屏蔽位 */
	
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
	struct gpio *const gp = get_base_gpio();
	struct interrupt *const intr = get_base_interrupt();
	int num = -1;


	num = intr->INTOFFSET;
	switch (num ) {
		case 0:
			printf("get interrupt 0.\n");
			break;
		case 1:
			printf("get interrupt 1.\n");
			break;
		case 2:
			printf("get interrupt 2.\n");
			break;
		case 3:
			printf("get interrupt 3.\n");
			break;
		default:
			printf("get unknow interrupt.\n");
			break;
	}

	/* 清除信号 */
	setval(intr->SRCPND, 0x1, 1,num);
	setval(intr->INTPND, 0x1, 1,num);
}
