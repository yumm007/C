#include <common.h>
#include <interrupt.h>
#include <s3c2440.h>

static void (*irq_handlers[IRQNUMBER])(void);

void irq_init(void)
{
	struct interrupt *const irq = get_base_interrupt();
	irq->INTMSK = ~0;

	__asm__ __volatile__ (
		"mrs	r0, cpsr\n\t"
		"bic	r0, r0, #0x80\n\t"
		"msr	cpsr, r0\n\t"
		:
		:
		:"r0"
	);
}


void request_irq(int num, void (*handler)(void))
{
	struct interrupt *const irq = get_base_interrupt();

	if(num >= 0 && num < IRQNUMBER && irq_handlers[num] == 0)
	{
		irq_handlers[num] = handler;

		set0(irq->INTMSK, num);
	}
}

void do_irq(void)
{
	struct interrupt *const irq = get_base_interrupt();

	const int num = irq->INTOFFSET & 0x1f;

	if(irq_handlers[num])
		irq_handlers[num]();

	irq->SRCPND = irq->INTPND;
	irq->INTPND = irq->INTPND;
}

void do_interrupt(int num, int swi)
{
	switch(num){
		case 1:
			puts("undefined_instruction");
			break;
		case 2:
			printf("software_interrupt, swi = %d\n", swi);
			return;
		case 3:
			puts("prefetch_abort");
			break;
		case 4:
			puts("data_abort");
			break;
		case 6:
			puts("fiq_interrupt");
			break;
	}
	while(1);
}

