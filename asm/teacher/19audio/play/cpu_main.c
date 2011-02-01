#include "s3c2440.h"

void iis_init(void)
{
	struct i2s *const i2s = get_base_i2s();

	i2s->IISCON = 0x2;

	i2s->IISMOD = 0x8d;

	i2s->IISPSR = 0x42;

	i2s->IISFCON = 0x3000;
}

int main(void)
{
	struct i2s *const i2s = get_base_i2s();
	unsigned short *buf = (void *)0x31000000;
	unsigned int i;

	wm8976_init();
	iis_init();

	i2s->IISCON |= 1;

	while(1)
	{
		if(!(i2s->IISCON & 0x80))
		{
			i2s->IISFIFO = *buf;
			buf++;	
		}
	}
	
}

