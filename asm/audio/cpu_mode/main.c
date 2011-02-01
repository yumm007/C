#include "s3c2440.h"
#include "common.h"

#define 	BUF		0x30000000
#define	BUFSIZE	1000000


void init_i2s(void) {
	struct i2s *const i2s = get_base_i2s();

	setval(i2s->IISCON,0x1,1,1);
	setval(i2s->IISMOD,0x2,2,6);
	setval(i2s->IISMOD,0xd,4,0);
	setval(i2s->IISPSR,0x2,5,5); /* 50 * 1024 / 384 / 44.1 = 3, = IISPSR + 1 */
	setval(i2s->IISFCON,0x1,1,13); /* Transfer FIFO mode */
}

void init_wm8976(void) {
	;
}

int main(void)
{
	struct i2s *const i2s = get_base_i2s();
	unsigned short int *buf = (void *)BUF;
	int i = 0;

	init_i2s();
	wm8976_init();
	i2s->IISCON |= 1;

	for (i = 0; i < BUFSIZE; i++) {
		while (i2s->IISCON & (1 << 7))
			;
		i2s->IISFIFO = buf[i];
	}

	return 0;
}
