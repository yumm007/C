#include "mini_fs.h"
#include <stdio.h>
#include <string.h>

#ifdef FS_DISK_RAM_FLASH
/*******************************************************
***	底层IO函数
***	使用RAM模拟硬盘，主要用于测试，
***   使用一个字节映射区，判断写入前对于的字节是否被擦除过
*******************************************************/

#define DISK_SPACE   SEGMENT_SIZE*DISK_BLOCK

static const BYTE __DISK[DISK_SPACE];
BYTE	__DISK_MAP[DISK_SPACE];
const BYTE *DISK = __DISK;

//需要移植的函数, 调用者确保地址已经按segment对齐
bool segment_erase(WORD seg_addr) {
	memset((char *)&DISK[seg_addr], 0, SEGMENT_SIZE);
	memset((char *)&__DISK_MAP[seg_addr], 0, SEGMENT_SIZE);
	//fprintf(stderr, "erase %lu\n", seg_addr);
	return true;
}

//需要移植的函数，实现将数据从FLASH拷贝到内存中
bool segment_read(WORD seg_addr, WORD offset, WORD buf, WORD len) {
	memcpy((char *)buf, (char *)&DISK[seg_addr+offset], (int)len);
	return true;
}

//需要移植的函数，将数据从内存写到FLASH，调用者保证所在的FLASH已经被擦过且操作不跨段
bool segment_write(WORD seg_addr, WORD offset,  WORD data, WORD len) {
	int i;
	char *flash_ptr = (char *)&DISK[seg_addr + offset];
	
	for (i = 0; i < len; i++) {
		if (__DISK_MAP[seg_addr + offset + i] == 1) {
			fprintf(stderr, "write %lu + %lu + %lu before erase\n", seg_addr, offset, len);
			return false;
		}
	}

	memcpy(flash_ptr, (char *)data, (int)len);
	memset(&__DISK_MAP[seg_addr + offset], 1, (int)len);
	//fprintf(stderr, "set %lu + %lu + %lu has used\n", seg_addr, offset, len);
	return true;
}

#endif // FS_DISK_ROM_FLASH
