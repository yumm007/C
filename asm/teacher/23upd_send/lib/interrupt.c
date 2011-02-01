#include <common.h>
#include <s3c2440.h>

void (*handler[32])(void);

void irq_init(void)
{
	struct interrupt *const irq = get_base_interrupt();

	irq->INTMSK = ~0;

	__asm__ __volatile__ (
		"mrs	r0, cpsr\n\t"
		"bic	r0, r0, #0x80\n\t"
		"msr	cpsr, r0\n\t"
	);
}

int request_irq(int no, void (*func)(void))
{
	if(no >= 0 && no <= 31 && handler[no] == NULL)
	{
		struct interrupt *const irq = get_base_interrupt();
		
		handler[no] = func;

		set0(irq->INTMSK, no);
		
		return 0;
	}

	return -1;
}

void do_irq(void)
{
	struct interrupt *const irq = get_base_interrupt();

	int no = irq->INTOFFSET & 0x1f;

	if(handler[no])
		handler[no](); 

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
		case 5:
			puts("irq_interrupt");
			break;
		case 6:
			puts("fiq_interrupt");
			break;
	}
	while(1);
}

