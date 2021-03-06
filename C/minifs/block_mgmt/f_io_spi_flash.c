#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mini_fs.h"

/*******************************************************
***	底层IO函数
***	需要实现Flash擦除函数，内存到Flash的复制，
***  flash到flash的复制共3个函数
*******************************************************/

#ifdef FS_DISK_SPI_FLASH
#include <msp430f149.h>
#include "flash.h"
FLASHID testid[2]={{0x00112233}};

//需要移植的函数, 调用者确保地址已经按segment对齐
bool segment_erase(WORD seg_addr) {
	FLASH_Erase(testid, FLASH_EREASE_SECTOR, seg_addr);
	return true;
}

//需要移植的函数，实现将数据从FLASH拷贝到内存中
bool segment_read(WORD seg_addr, WORD offset, WORD buf, WORD len) {
	UINT32 addr = FS_DISK_ADDR + seg_addr + offset;
	BYTE *p = (BYTE *)buf;
	FLASH_Read(testid, FLASH_FASTREAD, addr, p, len);
	return true;
}

//需要移植的函数，将数据从内存写到FLASH，
//调用者保证所在的FLASH已经被擦过且操作不超过最大写入单元
bool segment_write(WORD seg_addr, WORD offset,  WORD data, WORD len) {
	UINT32 addr = FS_DISK_ADDR + seg_addr + offset;
	BYTE *p = (BYTE *)data;
	FLASH_PageProgram(testid, addr, p, len);
	return true;
}

#endif // FS_DISK_SPI_FLASH
