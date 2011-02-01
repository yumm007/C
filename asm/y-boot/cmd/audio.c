#include <s3c2440.h>
#include <common.h>
#include <ushell.h>

#define 	BUF		0x30000000
#define	BUFSIZE	0x1000000


int audio(int argc, char **argv);
struct buidin_fun audio_fun  __attribute__ ((unused,section (".fun_section"))) = {"audio",audio};

static void init_i2s(void) {
	struct i2s *const i2s = get_base_i2s();

	setval(i2s->IISCON,0x1,1,1);
	setval(i2s->IISMOD,0x2,2,6);
	setval(i2s->IISMOD,0xd,4,0);
	setval(i2s->IISPSR,0x2,5,5); /* 50 * 1024 / 384 / 44.1 = 3, = IISPSR + 1 */
	setval(i2s->IISFCON,0x1,1,13); /* Transfer FIFO mode */
}

static void nsleep(unsigned int loops)
{
	loops *= 100;

	__asm__ __volatile__ (
		"1:\n\t"
		"subs %0, %1, #1\n\t"
		"bne 1b"
		:"=r"(loops)
		:"0"(loops)
	);
}

#define L3M	(1 << 2)
#define L3D	(1 << 3)
#define L3C	(1 << 4)

static void l3_write(unsigned short no, unsigned short val)
{
	unsigned int i;

	struct gpio *const gpio = get_base_gpio();

	unsigned short data = ((no & 0x7f) << 9) | (val & 0x1ff);

	gpio->GPBDAT |= L3M;
	gpio->GPBDAT |= L3C;
	
	nsleep(100);

	for(i = 0; i < 16; i++)
	{
		gpio->GPBDAT &= ~L3C;

		if(data & 0x8000)
			gpio->GPBDAT |= L3D;
		else
			gpio->GPBDAT &= ~L3D;
		
		nsleep(100);

		gpio->GPBDAT |= L3C;
		
		nsleep(100);

		data <<= 1;
	}

	gpio->GPBDAT &= ~L3M;
	nsleep(100);
	gpio->GPBDAT |= L3M;
}

	
static void wm8976_init(void)
{
	struct gpio *const gpio = get_base_gpio();

	gpio->GPBCON &= ~0x3f0;
	gpio->GPBCON |= 0x150;
	gpio->GPBUP |= 0x1c;

	l3_write(0, 0);
	l3_write(1, 0xf);
	l3_write(2, 0x180);
	l3_write(3, 0x6f);
	l3_write(4, 0x10);
	l3_write(6, 0x0);

	l3_write(7, 0x1);
	l3_write(9, 0x50);
	l3_write(13, 0x21);
}


int audio(int argc, char *argv[])
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

