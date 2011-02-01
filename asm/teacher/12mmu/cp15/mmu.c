

void (*printf)(const char *, ...) = (void *)0x33f9291c;

void _start(void)
{
	int c0_0;
	int c0_1;

	__asm__ __volatile__ (
		"mrc	p15,0,%[A],c0,c0,0\n\t"
		
		"mrc	p15,0,%[B],c0,c0,1\n\t"
		
		:[A]"=r"(c0_0), [B]"=r"(c0_1)
	);

	printf("c0_0 = %08x\n", c0_0);
	printf("c0_1 = %08x\n", c0_1);
}


