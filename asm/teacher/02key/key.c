
#define GPFCON (*(volatile unsigned int *)0x56000050)
#define GPFDAT (*(volatile unsigned int *)0x56000054)

void (*printf)(const char *, ...) = (void *)0x33f9291c;
void mdelay(unsigned int loop);

void _start(void)
{
	GPFCON &= ~0xf;
	
	while(1)
	{
		if(!(GPFDAT & 0x1))
		{
			printf("sw111111111111\n");
			mdelay(20);
		}
		if(!(GPFDAT & 0x2))
		{
			printf("sw222222222222\n");
			mdelay(20);
		}
	}
}


void mdelay(unsigned int loop)
{
	loop *= 1000000;

	__asm__ __volatile__ (
	"0:\n\t"
		"subs	%0, %0, #1\n\t"
		"bne	0b\n\t"
		:"+r"(loop)
	);
}

