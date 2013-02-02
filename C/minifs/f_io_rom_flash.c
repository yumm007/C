#include "mini_fs.h"
#include <msp430f149.h>
#include <string.h>

#ifdef FS_DISK_ROM_FLASH
/*******************************************************
***	底层IO函数
***	需要实现Flash擦除函数，内存到Flash的复制，
***  flash到flash的复制共3个函数
*******************************************************/

#define DISK_SPACE   SEGMENT_SIZE*DISK_BLOCK

static const BYTE __DISK[DISK_SPACE] @ "MINI_FS";
const BYTE *DISK = __DISK;
const BYTE *f_disk_addr(void) {
	return DISK;
}

//需要移植的函数, 调用者确保地址已经按segment对齐
bool segment_erase(WORD seg_addr) {
	_DINT();
	char *flash_ptr = (char *)&DISK[seg_addr];
	
	while (FCTL3 & BUSY)		// busy = 1 表示还没擦除完	
		;	
	FCTL1 = FWKEY + ERASE;                    // Set Erase bit
	//FCTL2 = FWKEY + FSSEL_1 + FN4 + FN2;      // MCLK/20 for Flash Timing Generator
	FCTL2 = FWKEY + FSSEL_1 + 9;
	FCTL3 = FWKEY;                            // Clear Lock bit
	*flash_ptr = 0;                           // Dummy write to erase Flash segment
	while (FCTL3 & BUSY)								// busy = 1 表示还没擦除完	
		;
	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit	
		
	_EINT();
}

//需要移植的函数，实现将数据从FLASH拷贝到内存中
bool segment_read(WORD seg_addr, WORD offset, WORD buf, WORD len) {
	memcpy((BYTE *)buf, &DISK[seg_addr+offset], len);
}

//需要移植的函数，将数据从内存写到FLASH，调用者保证所在的FLASH已经被擦过且操作不跨段
#if 1
bool segment_write(WORD seg_addr, WORD offset,  WORD data, WORD len) {
	int i;
	
	_DINT();
	char *flash_ptr = (char *)&DISK[seg_addr + offset];
	char *flash_dst = (char *)data;
	
	while (FCTL3 & BUSY)		// busy = 1 表示还没擦除完	
		;	
	FCTL1 = FWKEY + WRT;                    // Set WRT bit for write operation
	//FCTL2 = FWKEY + FSSEL_1 + FN4 + FN2;   		 // MCLK/20 for Flash Timing Generator	
	FCTL2 = FWKEY + FSSEL_1 + 9;   		 // MCLK/20 for Flash Timing Generator
	FCTL3 = FWKEY; 
	
	for (i = 0; i < len; i++) {
		flash_ptr[i] = flash_dst[i];                   // Write value to flash
		while (!(FCTL3 & WAIT))		// WAIT = 0 表示还没写完
			;
	}
	while (FCTL3 & BUSY)		// busy = 1 表示还没擦除完	
		;
	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
	_EINT();		
}
#else
__ramfunc void segment_write(WORD seg_addr, WORD offset, WORD data, WORD len) {
	int i;
	
	_DINT();
	char *flash_ptr = (char *)&DISK[seg_addr + offset];
	char *value = (char *)data;
	
	while (FCTL3 & BUSY)		// busy = 1 表示还没擦除完	
		;	
	//FCTL2 = FWKEY + FSSEL_1 + FN4 + FN2;   		 // MCLK/20 for Flash Timing Generator	
	FCTL2 = FWKEY + FSSEL_1 + 9;
	FCTL3 = FWKEY; 
	FCTL1 = FWKEY + BLKWRT + WRT;                    // Set BLOCK, WRT bit for write operation
	
	for (i = 0; i < len; i++) {
		flash_ptr[i] = value[i];                   // Write value to flash
		while (!(FCTL3 & WAIT))		// WAIT = 0 表示还没写完
			;
	}
	FCTL1 = FWKEY;           // Clear WRT bit
	while (FCTL3 & BUSY)		// busy = 1 表示还没擦除完	
		;
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
	_EINT();		
}

#endif

#if 0
//需要移植的函数，将数据从FLASH拷贝到FLASH，
//调用者保证目标所在FLASH已经被擦除且操作不超过最大写入单元
void segment_copy_segment(WORD seg_des,WORD dst_off, WORD seg_src, WORD len) {
#if 1
	segment_write(seg_des + dst_off, 0, (WORD)&DISK[seg_src], len);
#else
	int i;
	
	_DINT();
	char *flash_ptr = (char *)&DISK[seg_des + dst_off];
	char *flash_dst = (char *)&DISK[seg_src];
	
	while (FCTL3 & BUSY)		// busy = 1 表示还没擦除完	
		;	
	FCTL1 = FWKEY + WRT;                    // Set WRT bit for write operation
	//FCTL2 = FWKEY + FSSEL_1 + FN4 + FN2;   		 // MCLK/20 for Flash Timing Generator	
	FCTL2 = FWKEY + FSSEL_1 + 9;   		 // MCLK/20 for Flash Timing Generator
	FCTL3 = FWKEY; 
	
	for (i = 0; i < len; i++) {
		flash_ptr[i] = flash_dst[i];                   // Write value to flash
		while (!(FCTL3 & WAIT))		// WAIT = 0 表示还没写完
			;
	}
	while (FCTL3 & BUSY)		// busy = 1 表示还没擦除完	
		;
	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
	_EINT();		
#endif
}
#endif

#endif // FS_DISK_ROM_FLASH