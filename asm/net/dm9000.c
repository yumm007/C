#include "dm9000.h"

#define VENDER 	0x0a469000
#define SEND_WAIT 2000
#define VENDER_ID ((ior(0x29) << 24) | (ior(0x28) << 16) | (ior(0x2b) << 8) | ior(0x2a))

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
	
	if (VENDER_ID != VENDER) {
		printf("can't detached DM9000.\n");
		return;
	}

	iow(0xff, 1 << 7); /*启用 PAR*/
	iow(0x05, 1 | (1 << 4)); /*启用接收包功能，关闭CRC校验*/
}

int dm9000_recv(char *buf) {
	unsigned short status, len, i, *p, ret;

	if (!(ior(0xfe) & 0x1))
		return -1; /*无数据包可读时返回*/
	iow(0xfe, 0x0);
	if ((ior(0xf0) & 0x3) != 0x01) /*预读第一个字节发到data线上*/
		return -1; /*无效数据*/
	
	/*发读信号。 之后会把一个包的数据陆续发到16位的数据线上*/
	*(volatile unsigned char *)DM9000_IO =  0xf2;
	status = *(volatile unsigned short *)DM9000_DATA;
	ret = *(volatile unsigned short *)DM9000_DATA;
	p = (void *)buf;

	/* 共传输 (len +1) >> 1 */
	len = (ret + 1) >> 1;
	for (i = 0; i < len; i++)
		p[i] = *(volatile unsigned short *)DM9000_DATA;
	return (int)ret;
}

int dm9000_send(char *buf, unsigned int len) {
	unsigned char tmp_c;
	unsigned short *p, i, tmp_len;

   //清空发送状态标记
	tmp_c = ior(0xfe);
	iow(0xfe, tmp_c | 0x2);
   
   //发写信号 到 DM9000_IO;
	*(volatile unsigned char *)DM9000_IO = 0xf8;
   //发送包数据
	p = (void *)buf;
	tmp_len = (len + 1) >> 1;
	for (i = 0; i < tmp_len; i++)
		*(volatile unsigned short *)DM9000_DATA = p[i];
   
   //发送长度
	iow(0xfd, (len >> 8) & 0xFF);
	iow(0xfc, len & 0xFF);
   
   //申请发送
	tmp_c = ior(0x02);
	iow(0x02, tmp_c | 0x1);
   
	//等待发送成功后，清空发送状态标记
	i = 0;
	while (!(ior(0xfe) & 0x2)) {
		my_sleep(++i);
		if (i == SEND_WAIT)
			return -1;
	}
	tmp_c = ior(0xfe);
	iow(0xfe, tmp_c | 0x2);

	return 0;
}

