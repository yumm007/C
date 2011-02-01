
static void (*printf)(const char *, ...) = (void *)0x33f9291c;

void _start(void)
{
	char buf[2048];

	eth_init();

	while(1)
	{
		if(eth_recv(buf) > 0)
			printf("%s\n", buf + 42);
	}
}

void usleep(unsigned int loops)
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

