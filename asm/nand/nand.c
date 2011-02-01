#include "s3c2440.h"

#define NAND_CHIP_ENABLE 	nand_reg->NFCONT &= ~(1 << 1)
#define NAND_CHIP_DISABLE 	nand_reg->NFCONT |= 1 << 1
#define NAND_STATE_READY	while (!(nand_reg->NFSTAT & 1))

#define TACLS 	1
#define TWRPH0	3
#define TWRPH1	0

#define PAGE_SIZE	2048

typedef unsigned int volatile u32t;

static struct nand *nand_reg;

void nand_init() {
	nand_reg = get_base_nand();
	
	nand_reg->NFCONF = TACLS<<12|TWRPH0<<8|TWRPH1<<4;
	nand_reg->NFCONT = 0x73;

	NAND_CHIP_ENABLE;
	nand_reg->NFCMD = 0xff;
	NAND_STATE_READY;
	NAND_CHIP_DISABLE;
}

static void send_addr(u32t nand_add) {
	nand_reg->NFADDR = nand_add & 0xff;
	nand_reg->NFADDR = (nand_add >> 8 )& 0x7;

	nand_reg->NFADDR = (nand_add >> 11 )& 0xff;
	nand_reg->NFADDR = (nand_add >> 19 )& 0xff;
	nand_reg->NFADDR = (nand_add >> 27 )& 0x1;
}

u32t nand_read(u32t mem_add, u32t nand_add, u32t length){
	u32t read_c = 0, i;
	unsigned char *c = (unsigned char *)mem_add;
	
	NAND_CHIP_ENABLE;
	while (read_c < length) {
		nand_reg->NFCMD = 0x00;
		send_addr(nand_add + read_c);
		nand_reg->NFCMD = 0x30;
		NAND_STATE_READY;

		for (i = 0; i < PAGE_SIZE && read_c < length;\
			i++, read_c++) {
			c[read_c] =*(unsigned char volatile*)(&nand_reg->NFDATA);
		}
	}
	NAND_CHIP_DISABLE;

	return read_c;
}

void nand_erase(u32t addr, int length);

u32t nand_write(u32t mem_add, u32t nand_add, u32t length){
	u32t write_c = 0, i;
	unsigned char *c = (unsigned char *)mem_add;

	nand_erase(nand_add, length);	

	NAND_CHIP_ENABLE;
	while (write_c < length) {
		nand_reg->NFCMD = 0x80;
		send_addr(nand_add + write_c);

		for (i = 0; i < PAGE_SIZE && write_c < length;\
			i++, write_c++) {
			*(unsigned char volatile *)&nand_reg->NFDATA = c[write_c];
		}

		nand_reg->NFCMD = 0x10;
		NAND_STATE_READY;
	}
	NAND_CHIP_DISABLE;

	return write_c;
}

static inline void send_blockaddr(u32t addr)
{
	addr &= ~0x1ffff;

	nand_reg->NFADDR = (addr >> 11) & 0xff;
	nand_reg->NFADDR = (addr >> 19) & 0xff;
	nand_reg->NFADDR = (addr >> 27) & 0x1;
}

void nand_erase(u32t addr, int len)
{
	u32t block_cur;

	NAND_CHIP_ENABLE;
	for (block_cur = addr; block_cur < addr + len; block_cur += 0x20000) {
		nand_reg->NFCMD = 0x60;
		send_blockaddr(block_cur);	
		nand_reg->NFCMD = 0xd0;
		NAND_STATE_READY;
	}
	NAND_CHIP_DISABLE;
}

