#include "mini_fs.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef FS_DISK_RAM_FLASH
/*******************************************************
***	底层IO函数
***	使用RAM模拟硬盘，主要用于测试，
***   使用一个字节映射区，判断写入前对于的字节是否被擦除过
*******************************************************/
#define SIZEOF(s,m) ((size_t) sizeof(((s *)0)->m)) 

#define DISK_SPACE   SEGMENT_SIZE*DISK_BLOCK

fs_t fs = { 
   .flag = 0,
   .file = { 
      {offsetof(FILE_LEN_TABLE, FILE1_SIZE),   0, SIZEOF(FILE_LEN_TABLE, FILE1_SIZE)},
      {offsetof(FILE_LEN_TABLE, FILE2_SIZE),   0, SIZEOF(FILE_LEN_TABLE, FILE2_SIZE)},
      {offsetof(FILE_LEN_TABLE, FILE3_SIZE),   0, SIZEOF(FILE_LEN_TABLE, FILE3_SIZE)},
      {offsetof(FILE_LEN_TABLE, FILE4_SIZE),   0, SIZEOF(FILE_LEN_TABLE, FILE4_SIZE)},
      {offsetof(FILE_LEN_TABLE, FILE5_SIZE),   0, SIZEOF(FILE_LEN_TABLE, FILE5_SIZE)},
      {offsetof(FILE_LEN_TABLE, FILE6_SIZE),   0, SIZEOF(FILE_LEN_TABLE, FILE6_SIZE)},
      {offsetof(FILE_LEN_TABLE, FILE7_SIZE),   0, SIZEOF(FILE_LEN_TABLE, FILE7_SIZE)},
      {offsetof(FILE_LEN_TABLE, FILE8_SIZE),   0, SIZEOF(FILE_LEN_TABLE, FILE8_SIZE)},
      {offsetof(FILE_LEN_TABLE, FILE9_SIZE),   0, SIZEOF(FILE_LEN_TABLE, FILE9_SIZE)},
   },     
};


static const BYTE __DISK[DISK_SPACE];
const BYTE *DISK = &__DISK[0];
BYTE	__DISK_MAP[DISK_SPACE];
const BYTE *f_disk_addr(void) {
	return __DISK;
}

//需要移植的函数, 调用者确保地址已经按segment对齐
void segment_erase(WORD seg_addr) {
	memset((char *)&DISK[seg_addr], 0, SEGMENT_SIZE);
	memset((char *)&__DISK_MAP[seg_addr], 0, SEGMENT_SIZE);
	fprintf(stderr, "erase %lu\n", seg_addr);
}

//需要移植的函数，实现将数据从FLASH拷贝到内存中
void segment_read(WORD seg_addr, WORD offset, WORD buf, WORD len) {
	//fprintf(stderr, "%s(%lu, %lu, data, %lu)\n", __FUNCTION__, seg_addr, offset, len);
	memcpy((char *)buf, (char *)&DISK[seg_addr+offset], (int)len);
}

//需要移植的函数，将数据从内存写到FLASH，调用者保证所在的FLASH已经被擦过且操作不跨段
void segment_write(WORD seg_addr, WORD offset,  WORD data, WORD len) {
	int i;
	char *flash_ptr = (char *)&DISK[seg_addr + offset];
	//fprintf(stderr, "%s(%lu, %lu, data, %lu)\n", __FUNCTION__, seg_addr, offset, len);

	for (i = 0; i < len; i++) {
		if (__DISK_MAP[seg_addr + offset + i] == 1) {
			fprintf(stderr, "write %lu + %lu + %lu before erase\n", seg_addr, offset, len);
		}
	}

	memcpy(flash_ptr, (char *)data, (int)len);
	memset(&__DISK_MAP[seg_addr + offset], 1, (int)len);
	//fprintf(stderr, "set %lu + %lu + %lu has used\n", seg_addr, offset, len);
}

#endif // FS_DISK_ROM_FLASH
