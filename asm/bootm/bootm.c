struct tag_start{
	unsigned int a;
	unsigned int b;
	unsigned int c1, c2, c3;
};

struct tag_cmd{
	unsigned int a;
	unsigned int b;
	char cmd[104];
};

struct tag_end{
	unsigned int a;
	unsigned int b;
};

struct ker_arg{
	struct tag_start arg1;
	struct tag_cmd	  arg2;
	struct tag_end	  arg3;
};



void cp_to(char *, int);
void _start(void)
{
	void  (*theKernel)(unsigned int, unsigned int, unsigned int) = (void *)0x30008040;
	struct ker_arg arg = {{5, 0x54410001,0,0,0},\
							 {29,0x54410009,"noinitrd console=ttySAC0,115200 mem=64M root=/dev/nfs rw nfsroot=192.168.1.71:/nfsroot  ip=192.168.1.70"},\
							 {0,0x00000000}};

	cp_to((char *)&arg, sizeof(arg));
	theKernel(0, 362, 0x32000000);
}

void cp_to(char *sour, int len) {
	char *dest = (void *)0x32000000;
	int i;
	for (i = 0; i < len; i++)
		dest[i] = sour[i];
}

