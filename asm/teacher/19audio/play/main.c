#include "s3c2440.h"

void iis_init(void)
{
	struct i2s *const i2s = get_base_i2s();

	i2s->IISCON = 0x22;
	i2s->IISMOD = 0x8d;
	i2s->IISPSR = 0x42;
	i2s->IISFCON = 0xa000;
}

void dma_init(void)
{
	struct dmas *const dmas = get_base_dmas();
	struct i2s *const i2s = get_base_i2s();
		
	dmas->dma[0].DISRC = 0x30100000;
	dmas->dma[0].DISRCC = 0;

	dmas->dma[0].DIDST = (int)&i2s->IISFIFO;
	dmas->dma[0].DIDSTC = 3;

	dmas->dma[0].DCON = 0x25900000 | 512 * 512;
}

void open_irq(void)
{
	struct interrupt *const irq = get_base_interrupt();

	irq->INTMSK &= ~(1 << 17);

	__asm__ __volatile__ (
		"mrs	r0, cpsr\n\t"
		"bic	r0, r0, #0x80\n\t"
		"msr	cpsr, r0\n\t"
		:::"r0"
	);
}

int main(void)
{
	struct i2s *const i2s = get_base_i2s();
	struct dmas *const dmas = get_base_dmas();
	int i;
	char *dst = (void *)0x30100000;
	char *src = (void *)0x31000000;

	for(i = 0; i < 1024 * 1024; i++)
		dst[i] = src[i];

	wm8976_init();
	iis_init();
	dma_init();
	open_irq();

	dmas->dma[0].DMASKTRIG = 2;
	
	i2s->IISCON |= 1;
}

