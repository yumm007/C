#include <common.h>
#include <s3c2440.h>
#include <interrupt.h>

#include "net.h"

#define ICMP_SIZE 16

void icmp_unpack(u32 length)
{
	struct iphdr *ip;
	struct icmphdr *icmp;
	int ck_sum;

	ip = (void *)(recv_buf + sizeof(eth_t));

	icmp = (void *)(ip + 1);

	if(icmp->id != ntohs(0x1234))
		return;

	if(icmp->type != ICMP_ECHOREPLY)
		return;

	ck_sum = icmp->checksum;
	icmp->checksum = 0;

	if(ck_sum != cal_sum((void *)icmp, ICMP_SIZE))
		return;

	char sip[20];
	ip_to_string(ip->saddr, sip),

	printf("%d bytes from %s: icmp_seq=%d ttl=%d times= %8s\n",
		(ip->ihl << 2) + ICMP_SIZE,
		sip,
		icmp->sequence,
		ip->ttl,
		(char *)icmp + 8);
}

int icmp_pack(int seq)
{
	icmp_t *icmp = (void *)(send_buf + sizeof(eth_t) + sizeof(ip_t));

	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->id = htons(0x1234);
	icmp->sequence = seq;
	
	memcpy(icmp + 1, "12345678", 8);
	
	icmp->checksum = cal_sum((void *)icmp, ICMP_SIZE);

	return ip_pack(ICMP_SIZE, 1);
}

void ping(void)
{
	static int seq = 0;

	while(1)
	{
		seq++;
		icmp_pack(seq);
		sleep(10);
	}
}


