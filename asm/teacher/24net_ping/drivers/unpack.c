#include <common.h>

#include "net.h"

void eth_unpack(u32 length)
{
	eth_t *eth = (void *)recv_buf;

	if(memcmp(eth->dest, eeth, 6) != 0)
		return;

	switch(ntohs(eth->proto))
	{
		case 0x0800:
			ip_unpack(length - sizeof(eth_t));
			break;
	}
}

void ip_unpack(u32 length)
{
	ip_t *ip = (void *)(recv_buf + sizeof(eth_t));

	if(ip->daddr != eip)
		return;

	switch(ip->protocol)
	{
		case 1:
			icmp_unpack(length - sizeof(ip_t));
			break;
		case 17:
			udp_unpack(length - sizeof(ip_t));
			break;
	}
}

void udp_unpack(u32 length)
{

}

