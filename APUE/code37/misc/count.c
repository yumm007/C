#include <stdio.h>

static int
count(int n)
{
	n = (n&0x55555555)+((n&0xaaaaaaaa)>>1);
	n = (n&0x33333333)+((n&0xcccccccc)>>2);
	n = (n&0x0f0f0f0f)+((n&0xf0f0f0f0)>>4);
	n = (n&0x00ff00ff)+((n&0xff00ff00)>>8);
	n = (n&0x0000ffff)+((n&0xffff0000)>>16);
	return n;
}

int
main()
{
	int i;

	printf("static int data[256]={");
	for (i=0;i<256;++i) {
		printf("%d,", count(i));
	}
	printf("};\n");
	return 0;
}

