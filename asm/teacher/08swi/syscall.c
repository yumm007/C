
void (*printf)(const char *, ...) = (void *)0x33f9291c;

void open(void)
{
	printf("open....\n");
}

void read(void)
{
	printf("read....\n");
}


void syscall(int no)
{
	switch(no)
	{
		case 5:
			open();
			break;
		case 3:
			read();
			break;
	}
}










