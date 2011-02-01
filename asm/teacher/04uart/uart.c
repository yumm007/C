
void _start(void)
{
	char buf[1024];

	serial_init();

	gets(buf);

	puts(buf);
}


