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

//需要移植的函数, 调用者确保地址已经按segment对齐
bool segment_erase(WORD seg_addr) {
	memset((char *)seg_addr, 0xff, SEGMENT_SIZE);
	return true;
}

//需要移植的函数，实现将数据从FLASH拷贝到内存中
bool segment_read(WORD addr, WORD buf, WORD len) {
	//fprintf(stderr, "%s(%lu, %lu, data, %lu)\n", __FUNCTION__, seg_addr, offset, len);
	memcpy((char *)buf, (char *)(addr), (int)len);
	return true;
}

//需要移植的函数，将数据从内存写到FLASH，调用者保证所在的FLASH已经被擦过且操作不跨段
bool segment_write(WORD addr,  WORD data, WORD len) {
	WORD i;
	//fprintf(stderr, "%s(%lu, data, %lu)\n", __FUNCTION__, addr, len);
	for (i = 0; i < len; i++)
		((BYTE *)addr)[i] &= ((BYTE *)data)[i];
	return true;
}

#endif // FS_DISK_ROM_FLASH
