
#define GPECON (*(volatile unsigned int *)0x56000040)
#define GPEDAT (*(volatile unsigned int *)0x56000044)
#define GPFCON (*(volatile unsigned int *)0x56000050)
#define GPFDAT (*(volatile unsigned int *)0x56000054)

void delay(unsigned int);
void poll_key(void);

static int time = 5;

void (*printf)(const char *, ...) = (void *)0x33f9291c;

void _start(int argc, char *argv[])
{
	int i;

	GPECON &= ~(0xf << 24);
	GPECON |= (5 << 24);

	while(1)
	{
		GPEDAT &= ~(1 << 12);
		GPEDAT |= (1 << 13);


		for(i = 0; i < time; i++)
		{
			delay(time * 50);
			poll_key();
		}

		GPEDAT &= ~(1 << 13);
		GPEDAT |= (1 << 12);
		
		for(i = 0; i < time; i++)
		{
			delay(time * 50);
			poll_key();
		}
	}
}

void delay(unsigned int loops)
{
	loops *= 100000;
	__asm__ __volatile__ (
	"0:\n\t"
		"subs	%0, %0, #1\n\t"
		"bne	0b\n\t"
		:"+r"(loops)
	);
}

void poll_key(void)
{
	if(!(GPFDAT & 0x1))
	{
		if(time < 10)
			time++;

		delay(5);

		printf("\033[2J\033[Htime = %d\n", time);
	}

	if(!(GPFDAT & 0x2))
	{
		if(time > 1)
			time--;
		
		delay(5);

		printf("\033[2J \033[H time = %d\n", time);
	}

}
