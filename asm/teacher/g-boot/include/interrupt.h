#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#define IRQNUMBER	32

#define	EINT0		0
#define EINT1		1
#define EINT2		2
#define EINT3		3
#define EINT4_7		4
#define EINT8_23	5
#define INT_CAM		6
#define nBATT_FLT	7
#define INT_TICK	8
#define INT_WDT_AC97	9
#define INT_TIMER0	10
#define INT_TIMER1	11
#define INT_TIMER2	12
#define INT_TIMER3	13
#define INT_TIMER4	14
#define INT_UART2	15
#define INT_LCD		16
#define INT_DMA0	17
#define INT_DMA1	18
#define INT_DMA2	19
#define INT_DMA3	20
#define INT_SDI		21
#define INT_SPI0	22
#define INT_UART1	23
#define INT_NFCON	24
#define INT_USBD	25
#define INT_USBH	26
#define INT_IIC		27
#define INT_UART0	28
#define INT_SPI1	29
#define INT_RTC		30
#define INT_ADC		31

void irq_init(void);
void request_irq(int num, void(*handler)(void));
void do_irq(void);
void do_interrupt(int num, int swi);

#endif
