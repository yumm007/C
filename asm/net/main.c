
static void (*printf)(const char *, ...) = (void *)0x33f9291c;

void _start(void)
{
	char buf[2048];

	dm9000_init();
#if 0
	while(1)
	{
		if(dm9000_recv(buf) > 0)
			printf("%s\n", buf + 42);
	}
#else
	if (dm9000_send("send from 1.70", 15) != 0)
		printf("send failed.\n");
	else 
		printf("send ok.\n");
#endif
}

