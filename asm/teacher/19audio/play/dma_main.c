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
		
	dmas->dma[0].DISRC = 0x31000000;
	dmas->dma[0].DISRCC = 0;

	dmas->dma[0].DIDST = (int)&i2s->IISFIFO;
	dmas->dma[0].DIDSTC = 3;

	dmas->dma[0].DCON = 0x85d00000 | 0xfffff;
}

int main(void)
{
	struct i2s *const i2s = get_base_i2s();
	struct dmas *const dmas = get_base_dmas();

	wm8976_init();
	iis_init();
	dma_init();

	dmas->dma[0].DMASKTRIG = 2;
	
	i2s->IISCON |= 1;
}

