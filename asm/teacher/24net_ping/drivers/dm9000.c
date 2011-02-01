#include <common.h>
#include "dm9000.h"
#include "net.h"

u8 dm9000_ior(u8 reg)
{
	*(volatile u8 *)DM9000_IO = reg;
	return *(volatile u8 *)DM9000_DATA;
}

void dm9000_iow(u8 reg, u8 data)
{
	*(volatile u8 *)DM9000_IO = reg;
	*(volatile u8 *)DM9000_DATA = data;
}

void eth_init(void)
{
	dm9000_iow(DM9000_GPR, 0x0);
	dm9000_iow(DM9000_NCR, 0x1);
	usleep(20);

	unsigned int id = 0;
	id = dm9000_ior(DM9000_VIDL);
	id |= dm9000_ior(DM9000_VIDH) << 8;
	id |= dm9000_ior(DM9000_PIDL) << 16;
	id |= dm9000_ior(DM9000_PIDH) << 24;
	if(id != 0x90000a46)
	{	
		printf("DM9000 not found...\n");
		return;
	}
	
	dm9000_iow(DM9000_IMR, IMR_PAR);

	dm9000_iow(DM9000_RCR, RCR_RXEN | RCR_DIS_CRC | RCR_DIS_LONG);
}


int eth_recv(char *pkt)
{
	u16 status;
	u16 length;
	u16 i;

	if(!(dm9000_ior(DM9000_ISR) & ISR_PRS))
		return -1;
	dm9000_iow(DM9000_ISR, ISR_PRS);

	if((dm9000_ior(DM9000_MRCMDX) & 0x3) != 0x01)
		return -1;

	*(volatile u8 *)DM9000_IO = DM9000_MRCMD;

	status = *(u16 *)DM9000_DATA;
	length = *(u16 *)DM9000_DATA;

	for(i = 0; i < ((length + 1) >> 1); i++)
		*((u16 *)pkt + i) =  *(volatile u16 *)DM9000_DATA;

	return length;
}

int eth_send(const char *pkt, u32 length)
{
	int i;

	//先禁止网卡中断，防止在发送数据时被中断干扰
	dm9000_iow(DM9000_IMR, IMR_PAR);


	dm9000_iow(DM9000_ISR, ISR_PTS);

	*(volatile u8 *)DM9000_IO = DM9000_MWCMD;

	for(i = 0; i < ((length + 1) >> 1); i++)
		*(volatile u16 *)DM9000_DATA = *((u16 *)pkt + i);


	dm9000_iow(DM9000_TXPLL, length & 0xff);
	dm9000_iow(DM9000_TXPLH, (length >> 8) & 0xff);
	
	dm9000_iow(DM9000_TCR, TCR_TXREQ);

	i = 0;
	while(!(dm9000_ior(DM9000_ISR) & ISR_PTS))
	{
		if(i > 10000)
		{
			puts("TX timeout...");
			return -1;
		}
		usleep(100);
		i++;
	}

	dm9000_iow(DM9000_ISR, ISR_PTS);

	//DM9000网卡的接收中断使能
	dm9000_iow(DM9000_IMR, IMR_PAR | IMR_PRM);

	return 0;
}

