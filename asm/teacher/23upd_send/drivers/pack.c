#include <common.h>
#include "net.h"

int eth_pack(u32 length, u16 type)
{
	eth_t *eth = (void *)send_buf;

	memcpy(eth->dest, heth, 6);
	memcpy(eth->source, eeth, 6);
	eth->proto = htons(type);

	return eth_send((void *)send_buf, length + sizeof(eth_t));
}

int ip_pack(u32 length, u8 type)
{
	ip_t *ip = (void *)(send_buf + sizeof(eth_t));

	ip->ihl = sizeof(ip_t) >> 2;
	ip->version = 4;
	ip->tos = 0;
	ip->tot_len = htons(sizeof(ip_t) + length);
	ip->id = 0;
	ip->frag_off = 0;
	ip->ttl = 255;
	ip->protocol = type;
	ip->check = 0;
	ip->saddr = eip;
	ip->daddr = hip;

	ip->check = cal_sum((void *)ip, sizeof(ip_t));

	return eth_pack(sizeof(ip_t) + length, 0x0800);
}

int udp_pack(u32 length)
{
	udp_t *udp = (void *)(send_buf + sizeof(eth_t) + sizeof(ip_t));

	udp->source = eport;
	udp->dest = hport;
	udp->len = htons(sizeof(udp_t) + length);
	udp->check = 0;

	return ip_pack(sizeof(udp_t) + length, 17);
}



