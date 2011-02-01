#include <common.h>
#include <s3c2440.h>
#include <command.h>

static struct nand *const nand = (struct nand *)NAND_BASE;
static int page_size = 0;
static int block_size = 0;
static int oob_size = 0;

#define NAND_CHIP_ENABLE	set0(nand->NFCONT, 1)
#define NAND_CHIP_DISABLE	set1(nand->NFCONT, 1)
#define CHECK_RB		while(!(nand->NFSTAT & 1))

#define TACLS	1	/*TACLS是当CLE/ALE有效时过了多少时间后nWE/nRE才有效*/
#define TWRPH0	3	/* TWRPH0是nWE/nRE的有效时间 */
#define TWRPH1	0	/* TWRPH1是当nWE/nRE无效后DATA的保持时间 */


void nand_init(void)
{
	nand->NFCONF = (TACLS << 12) | (TWRPH0 << 8) | (TWRPH1 << 4);

	nand->NFCONT = (1 << 4) | (1 << 1) | 1;

	NAND_CHIP_ENABLE;
	nand->NFCMD = 0xff;	//reset;
	CHECK_RB;
	NAND_CHIP_DISABLE;

	switch((nand->NFCONF >> 2) & 0x3) {
		case 1:
			page_size = 512;
			block_size = page_size * 32;
			oob_size = 16;
			break;
		case 3:
		default:
			page_size = 2048;
			block_size = page_size * 64;
			oob_size = 64;
			break;
	}
}

static inline void send_addr(u32 addr)
{
	nand->NFADDR = addr & 0xff;
	nand->NFADDR = (addr >> 8) & 0x7;
	nand->NFADDR = (addr >> 11) & 0xff;
	nand->NFADDR = (addr >> 19) & 0xff;
	nand->NFADDR = (addr >> 27) & 0x1;
}

static int nand_read(u32 dst_addr, u32 src_addr, int len)
{
	unsigned int i = 0;
	unsigned int page_cur = 0;
	
	if(page_size != 2048){
		printf("page size is not 2048\n");
		return -1;
	}

	if(src_addr & 0x7ff){
		printf("offset is not page alignment\n");
		return -1;
	}

	NAND_CHIP_ENABLE;
		
	for(page_cur = src_addr; page_cur < len + src_addr; 
			page_cur += page_size)
	{
		nand->NFCMD = 0x00;	//1st. Cycle

		send_addr(page_cur);

		nand->NFCMD = 0x30;	//2nd. Cycle
		
		CHECK_RB;

		for(i = 0; i < page_size; i += 4)
		{
			*(volatile u32 *)dst_addr = nand->NFDATA;
			dst_addr += 4;
		}

	}

	NAND_CHIP_DISABLE;

	return 0;
}

static int nand_write(u32 src_addr, u32 dst_addr, int len)
{
	unsigned int i = 0;
	unsigned int page_cur = 0;
	
	if(page_size != 2048){
		printf("page size is not 2048\n");
		return -1;
	}

	if(src_addr & 0x7ff){
		printf("offset is not page alignment\n");
		return -1;
	}

	NAND_CHIP_ENABLE;
		
	for(page_cur = dst_addr; page_cur < len + dst_addr; 
			page_cur += page_size)
	{
		nand->NFCMD = 0x80;	//1st. Cycle

		send_addr(page_cur);

		for(i = 0; i < page_size; i += 4)
		{
			nand->NFDATA = *(volatile u32 *)src_addr;
			src_addr += 4;
		}

		nand->NFCMD = 0x10;	//2nd. Cycle

		CHECK_RB;
	}

	NAND_CHIP_DISABLE;

	return 0;
}

static inline void send_blockaddr(u32 addr)
{
	addr &= ~0x1ffff;

	nand->NFADDR = (addr >> 11) & 0xff;
	nand->NFADDR = (addr >> 19) & 0xff;
	nand->NFADDR = (addr >> 27) & 0x1;
}

static int nand_erase(u32 addr, int len)
{
	u32 block_cur;

	if(page_size != 2048){
		printf("page size is not 2048\n");
		return -1;
	}

	if(addr & 0x1ffff){
		printf("offset is not block alignment\n");
		return -1;
	}

	NAND_CHIP_ENABLE;
	
	for(block_cur = addr; block_cur < addr + len; block_cur += 0x20000)
	{
		nand->NFCMD = 0x60;
		
		send_blockaddr(block_cur);	

		nand->NFCMD = 0xd0;

		CHECK_RB;
	}

	NAND_CHIP_DISABLE;

	return 0;
}

static int do_nand(int argc, char *argv[])
{
	if((argc == 5) && (!strcmp(argv[1], "read")))
	{
		if(nand_read(atoh(argv[2]), atoh(argv[3]), atoh(argv[4])))
			printf("nand read error.\n");
	}
	else if((argc == 5) && (!strcmp(argv[1], "write")))
	{
		if(nand_write(atoh(argv[2]), atoh(argv[3]), atoh(argv[4])))
			printf("nand write error.\n");
	}
	else if((argc == 4) && (!strcmp(argv[1], "erase")))
	{
		if(nand_erase(atoh(argv[2]), atoh(argv[3])))
			printf("nand erase error.\n");
	}

	return 0;
}

G_BOOT_CMD(nand, do_nand);


