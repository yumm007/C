#include "dm9000.h"

#define VENDER_ID ((io_r(0x29) << 24) | (io_r(0x28) << 16) | (ior(0x2b) << 8) | ior(0x2a))

static void (*printf)(const char *, ...) = (void *)0x33f9291c;

static iow(unsigned char reg, unsigned char val) {
	/*先发地址，再发数据*/
	*(volatile unsigned char *)DM9000_IO	= reg;
	*(volatile unsigned char *)DM9000_DATA	= val;
}

static unsigned char ior(unsigned char reg) {
	*(volatile unsigned char *)DM9000_IO	= reg;
	return *(volatile unsigned char *)DM9000_DATA;
}

static void my_sleep(unsigned int tim) {
	unsigned int n;
	tim = tim * 10000;
	while (tim--)
		n = tim;
}

void dm9000_init(void) {
	unsigned int id;	

	iow(DM9000_GPR, 0); /*power on*/
	iow(0x00, 1);
	my_sleep(20);
	
	if (VENDER_ID != 0x0a469000) {
		printf("can't detached DM9000.\n");
		return -1;
	}

	iow(0xff, 1 << 7); /*启用 PAR*/
	iow(0x05, 1 | (1 << 4)); /*启用接收包功能，关闭CRC校验*/
}

int dm9000_recv(char *buf) {
	unsigned short status, len, i, *p, ret;

	if (!(ior(0xfe) & 0x1))
		return -1; /*无数据包可读时返回*/
	iow(0xf3, 0x0);
	if ((ior(0xf0) & 0x3) != 0x01) /*预读第一个字节发到data线上*/
		return -1; /*无效数据*/
	
	/*正式读数据*/
	*(volatile unsigned char *)DM9000_IO =  0xf2;
	status = *(volatile unsigned char *)DM9000_DATA;
	ret = *(volatile unsigned char *)DM9000_DATA;
	p = (void *)buf;

	/* 数据线16位，一次可以传送16个位，共传输 (len +1) >> 1 */
	len = (ret + 1) >> 1;
	for (i = 0; i < len; i++)
		p[i] = *(volatile unsigned char *)DM9000_DATA;
	
	return ret;
}

