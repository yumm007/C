#include <common.h>
#include <s3c2440.h>
#include <interrupt.h>
#include "net.h"

u8 heth[6];
u8 eeth[6];

u32 hip;
u32 eip;

u16 hport;
u16 eport;

u8 send_buf[2048];
u8 recv_buf[2048];

static void irq_recv(void)
{
	u32 len;
	struct gpio *const gpio = get_base_gpio();
		
	if(!(gpio->EINTPEND & 0x80))
		return;

	if((len = eth_recv((void *)recv_buf)) > 0)
		eth_unpack(len);
		
	gpio->EINTPEND = 0x80;
}

int main(int argc, char *argv[])
{
	struct gpio *gpio = get_base_gpio();

	string_to_eth("00:58:5D:4C:32:25", heth);	
	string_to_eth("11:22:33:44:55:66", eeth);
	
	eip = string_to_ip("192.168.1.8");
	hip = string_to_ip("192.168.1.3");

	hport = htons(1234);
	eport = htons(4321);

	eth_init();

	setval(gpio->GPFCON, 2, 2, 14);
	set1(gpio->GPFUP, 7);

	set0(gpio->EINTMASK, 7);
	setval(gpio->EXTINT0, 1, 3, 28);
	
	request_irq(EINT4_7, irq_recv);

	ping();

	return 0;
}


