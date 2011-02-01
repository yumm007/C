#include "common.h"
#include "s3c2440.h"

#define NAND_CHIP_ENABLE	(nand->NFCONT &= ~0x2)
#define NAND_CHIP_DISABLE	(nand->NFCONT |= 0x2)
#define CHECK_RB		while(!(nand->NFSTAT & 1))

#define TACLS	0	/*TACLS是当CLE/ALE有效时过了多少时间后nWE/nRE才有效*/
#define TWRPH0	2	/* TWRPH0是nWE/nRE的有效时间 */
#define TWRPH1	0	/* TWRPH1是当nWE/nRE无效后DATA的保持时间 */


static void nand_init(void)
{
	struct nand *const nand = get_base_nand();

	nand->NFCONF = (TACLS << 12) | (TWRPH0 << 8) | (TWRPH1 << 4);
	nand->NFCONT = 3;


	NAND_CHIP_ENABLE;
	nand->NFCMD = 0xff;
	CHECK_RB;	
	NAND_CHIP_DISABLE;
}

static void send_addr(unsigned int addr)
{
	struct nand *const nand = get_base_nand();

	nand->NFADDR = addr & 0xff;
	nand->NFADDR = (addr >> 8) & 0x7;
	
	nand->NFADDR = (addr >> 11) & 0xff;
	nand->NFADDR = (addr >> 19) & 0xff;
	nand->NFADDR = (addr >> 27) & 1;
}

int nand_read_lowlevel(unsigned int dst_addr, 
		unsigned int src_addr, unsigned int len)
{
	unsigned int addr;
	unsigned int i;
	struct nand *const nand = get_base_nand();

	if(src_addr & 2047)
		return -1;

	nand_init();

	NAND_CHIP_ENABLE;

	for(addr = src_addr; addr < src_addr + len; addr += 2048)
	{
		nand->NFCMD = 0x00;	//1st. Cycle

		send_addr(addr);

		nand->NFCMD = 0x30;	//2nd. Cycle

		CHECK_RB;

		for(i = 0; i < 2048; i += 4)
		{
			*(volatile unsigned int *)dst_addr = nand->NFDATA;

			dst_addr += 4;
		}
	}

	NAND_CHIP_DISABLE;

	return 0;
}

