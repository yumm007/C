#include <common.h>
#include "net.h"


u8 heth[6];
u8 eeth[6];

u32 hip;
u32 eip;

u16 hport;
u16 eport;

u8 send_buf[2048];
u8 recv_buf[2048];

int main(int argc, char *argv[])
{
	string_to_eth("00:58:5D:4C:32:25", heth);
	string_to_eth("11:22:33:44:55:66", eeth);
	
	hip = string_to_ip("192.168.1.3");
	eip = string_to_ip("192.168.1.8");

	hport = htons(1234);
	eport = htons(4321);

	eth_init();

	char s[] = "123456789";

	memcpy(send_buf + 42, s, sizeof(s));

	while(1)
	{
		udp_pack(sizeof(s));
		sleep(10);
	}

	return 0;
}



