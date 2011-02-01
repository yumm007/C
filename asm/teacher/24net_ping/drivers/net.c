#include <common.h>
#include "net.h"

#define swap(x, y) {typeof(x) tmp = x; x = y; y = tmp;}

unsigned int swapl(unsigned int d)
{
	unsigned char *p = (unsigned char *)&d;
	swap(p[0], p[3]);
	swap(p[1], p[2]);
	return d;
}

unsigned short swaps(unsigned short d)
{
	unsigned char *p = (unsigned char *)&d;
	swap(p[0], p[1]);
	return d;
}

void ip_to_string(unsigned int x, char *s)
{
	x = ntohl(x);
	sprintf(s, "%d.%d.%d.%d",
		 (int) ((x >> 24) & 0xff),
		 (int) ((x >> 16) & 0xff),
		 (int) ((x >> 8) & 0xff), (int) ((x >> 0) & 0xff)
	);
}

unsigned int string_to_ip(const char *s)
{
	unsigned int addr;
	char *e;
	int i;

	if (s == NULL)
		return(0);

	for (addr=0, i=0; i<4; ++i) {
		unsigned int val = simple_strtoul(s, &e, 10);
		addr <<= 8;
		addr |= (val & 0xFF);
		s = *e ? e+1 : e;
	}

	return (htonl(addr));
}

void string_to_eth(const char *s, unsigned char *e)
{
	int i;
	char *end;

	for(i = 0; i < 6; i++)
	{
		e[i] = simple_strtoul(s, &end, 16);
		s = *end ? end + 1 : end;
	}
}


unsigned short cal_sum(unsigned short *buf, int len)
{
	unsigned int sum = 0;

	while(len > 1)
	{
		sum += *buf;
		buf++;
		len -= 2;
	}

	if(len == 1)
		sum += *(unsigned char *)buf;

	sum = (sum >> 16) + (sum & 0xffff);

	sum += (sum >> 16);


	return (~sum) & 0xffff;
}


