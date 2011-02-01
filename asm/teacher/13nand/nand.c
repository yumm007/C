#include "s3c2440.h"

typedef unsigned int u32;
typedef unsigned char u8;

#define NAND_CHIP_ENABLE	(nand->NFCONT &= ~(1 << 1))
#define NAND_CHIP_DISABLE	(nand->NFCONT |= (1 << 1))
#define CHECK_RB		while(!(nand->NFSTAT & 0x1))

#define TACLS	0	/*TACLS是当CLE/ALE有效时过了多少时间后nWE/nRE才有效*/
#define TWRPH0	1	/* TWRPH0是nWE/nRE的有效时间 */
#define TWRPH1	0	/* TWRPH1是当nWE/nRE无效后DATA的保持时间 */


void nand_init(void)
{
	struct nand *const nand = get_base_nand();

	nand->NFCONF = (TACLS << 12) | (TWRPH0 << 8) | (TWRPH1 << 4);
	nand->NFCONT = 0x73;
	
	NAND_CHIP_ENABLE;
	nand->NFCMD = 0xff;  //reset
	CHECK_RB;
	NAND_CHIP_DISABLE;
}

static void send_addr(u32 addr)
{
	struct nand *const nand = get_base_nand();

	nand->NFADDR = addr & 0xff;
	nand->NFADDR = (addr >> 8) & 0x7;
	
	nand->NFADDR = (addr >> 11) & 0xff;
	nand->NFADDR = (addr >> 19) & 0xff;
	nand->NFADDR = (addr >> 27) & 0x1;

}

int nand_read(u32 dst_addr, u32 src_addr, u32 length)
{
	u32 addr;
	u32 i;
	struct nand *const nand = get_base_nand();

	NAND_CHIP_ENABLE;

	for(addr = src_addr; addr < src_addr + length; addr += 2048)
	{
		nand->NFCMD = 0x00;

		send_addr(addr);

		nand->NFCMD = 0x30;
		
		CHECK_RB;

		for(i = 0; i < 2048; i += 1)
		{
			*(u8 *)dst_addr = *(u8 *)&nand->NFDATA;
			dst_addr += 1;
		}
	}

	NAND_CHIP_DISABLE;

	return 0;
}


int nand_write(u32 src_addr, u32 dst_addr, u32 length)
{
	u32 addr;
	u32 i;
	struct nand *const nand = get_base_nand();

	NAND_CHIP_ENABLE;

	for(addr = dst_addr; addr < dst_addr + length; addr += 2048)
	{
		nand->NFCMD = 0x80;

		send_addr(addr);

		for(i = 0; i < 2048; i += 4)
		{
			nand->NFDATA = *(u32 *)src_addr;
			src_addr += 4;
		}

		nand->NFCMD = 0x10;
		
		CHECK_RB;
	}

	NAND_CHIP_DISABLE;

	return 0;
}

void send_block_addr(u32 addr)
{
	struct nand *const nand = get_base_nand();

	addr >>= 11;

	nand->NFADDR = addr & 0xc0;
	nand->NFADDR = (addr >> 8) & 0xff;
	nand->NFADDR = (addr >> 16) & 0x1;
}

int nand_erase(u32 dst_addr, u32 length)
{
	u32 addr;
	struct nand *const nand = get_base_nand();

	NAND_CHIP_ENABLE;

	for(addr = dst_addr; addr < dst_addr + length; addr += 0x20000)
	{
		nand->NFCMD = 0x60;

		send_block_addr(addr);

		nand->NFCMD = 0xd0;

		CHECK_RB;
	}

	NAND_CHIP_DISABLE;

	return 0;
}


