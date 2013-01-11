#include <stdio.h>
#include <string.h>

typedef unsigned char UINT8;
typedef unsigned int UINT16;

typedef enum {
	FILE_ID_BEGIN,

	FILE1	= FILE_ID_BEGIN,
	FILE2,
	FILE3,

	FILE_ID_END = FILE3,
} file_id_t;

struct file_info_t {
	file_id_t 	id;
	UINT16 		start_addr;
	UINT16		file_len;
};

static const struct file_info_t file_info[] = {
	{FILE1, 0, 10},
	{FILE2, 10, 23},
	{FILE3, 33, 150},
};


UINT8 * f_read(file_id_t id, 	UINT16 offset,	UINT16 len);
void 	f_write(file_id_t id, 	UINT16 offset,	const UINT8 *data, UINT16 len);
static void disk_write(UINT16 offset, const UINT8 *data, UINT16 len);

/*******************************************************
***	用户接口层次代码
***	read函数直接返回FLASH地址即可
*** write 函数需要进行FLASH擦写函数配合
*******************************************************/
UINT8 * f_read(file_id_t id, UINT16 offset, UINT16 len) {
	if (id < FILE_ID_BEGIN || id > FILE_ID_END)
		return NULL;
	if (offset + len > file_info[id].file_len)
		return NULL;
	return (UINT8 *)(file_info[id].start_addr + offset);
}

void f_write(file_id_t id, 	UINT16 offset,	const UINT8 *data, UINT16 len) {
	if (id < FILE_ID_BEGIN || id > FILE_ID_END)
		return;
	if (offset + len > file_info[id].file_len)
		return;
	
	disk_write(file_info[id].start_addr + offset, data, len);
}

/*******************************************************
***	底层IO函数
***	read，和write 函数
*******************************************************/

#define SEGMENT_SIZE	16
#define DISK_SPACE	SEGMENT_SIZE*50
#define SWAP_ADDR	SEGMENT_SIZE*49

static UINT8 DISK[DISK_SPACE];

static void f_dump(void) {
	int i, j, n;
	n = sizeof(file_info) / sizeof(file_info[0]);
	for (i = 0; i < n; i++) {
		printf("FILE %d: addr = %d, len = %d\n", i, file_info[i].start_addr, file_info[i].file_len);
		for (j = 0; j < file_info[i].file_len; j++)
			putchar(DISK[file_info[i].start_addr + j]);
		putchar('\n');
	}
}

static void segment_erase(UINT16 addr) {
	memset(&DISK[addr], '0', SEGMENT_SIZE);
}

static void segment_copy(UINT16 addr, const UINT8 *data, UINT16 len) {
	memcpy(&DISK[addr], data, len);
}

static void segment_only_write(UINT16 addr, const UINT8 *data) {
	segment_erase(addr);
	segment_copy(addr, data, SEGMENT_SIZE);
}

static void segment_write(UINT16 addr, UINT16 offset, const UINT8 *data, UINT16 len) {
	UINT16 n = offset + len;
	if (len == 0)
		return;

	segment_erase(SWAP_ADDR);
	//segment_copy(SWAP_ADDR, (UINT8 *)addr, offset);
	fprintf(stderr, "memcpy(%d, %d, %d)\n", SWAP_ADDR, addr, offset);
	memcpy(&DISK[SWAP_ADDR], &DISK[addr], offset);
	//segment_copy(SWAP_ADDR + n, (UINT8 *)(addr + n), SEGMENT_SIZE - n);
	fprintf(stderr, "memcpy(%d,%d,%d)\n", SWAP_ADDR + n, addr + n, SEGMENT_SIZE -n);
	memcpy(&DISK[SWAP_ADDR + n], &DISK[addr + n], SEGMENT_SIZE - n);

	segment_erase(addr);
	//segment_copy(addr, (UINT8 *)SWAP_ADDR, offset);
	fprintf(stderr, "memcpy(%d,%d,%d)\n", addr, SWAP_ADDR, offset);
	memcpy(&DISK[addr], &DISK[SWAP_ADDR], offset);
	fprintf(stderr, "memcpy(%d,%p,%d)\n",addr, data, len);
	segment_copy(addr + offset, data, len);
	//segment_copy(addr + n, (UINT8 *)SWAP_ADDR + n, SEGMENT_SIZE - n);
	fprintf(stderr, "memcpy(%d,%d,%d)\n",addr + n, SWAP_ADDR + n, SEGMENT_SIZE - n);
	memcpy(&DISK[addr + n], &DISK[SWAP_ADDR + n], SEGMENT_SIZE - n);
}

static void disk_write(UINT16 offset, const UINT8 *data, UINT16 len) {
	UINT16 temp_addr, temp_off, temp_len;
	int i, n;
	
	//第一步，offset向下对齐 - offset, 知道需要改写的上一块地址
	temp_addr = (offset / SEGMENT_SIZE ) * SEGMENT_SIZE;
	temp_off = offset - temp_addr;
	temp_len = SEGMENT_SIZE - temp_off > len ? len : SEGMENT_SIZE - temp_off;
	
	fprintf(stderr, "STEP1: segment_write(%d,%d,%p,%d)\n",temp_addr, temp_off, data, temp_len);
	segment_write(temp_addr, temp_off, data, temp_len);
	
	//第二步, copy 刚好对齐的部分
	offset = temp_addr + SEGMENT_SIZE;
	data += temp_len;
	len -= temp_len;
	
	n = len / SEGMENT_SIZE;
	//printf("block:%d / %d")
	for (i = 0; i < n; i++) {
		fprintf(stderr, "STEP2: segment_only_write(%d, %p)\n", offset + i * SEGMENT_SIZE, data + i * SEGMENT_SIZE);
		segment_only_write(offset + i * SEGMENT_SIZE, data + i * SEGMENT_SIZE);
	}
	
	//第三步
	offset += n * SEGMENT_SIZE;
	data += n * SEGMENT_SIZE;
	len -= n * SEGMENT_SIZE;
	fprintf(stderr, "STEP3: segment_write(%d,%d,%p,%d)\n",offset, 0, data, len);
	segment_write(offset, 0, data, len);
	
}

/*******************************************************
***	用户层测试函数
*******************************************************/


int main(void) {
	UINT8 i, tmp[17] = "this is a test";

	memset(DISK, '0', sizeof(DISK));
	for (i = 0; i < 7; i++) {
		f_write(FILE2, 1, tmp, sizeof(tmp));
		f_write(FILE3, 20, "abcdefg", 7);
		f_write(FILE3, 35, "1234567", 7);
		f_write(FILE3, 2, "ABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFG", 63);
		f_write(FILE3, 0, "ZZZ", 3);
		f_write(FILE1, 2, "ABCDEFGABC", 8);
		f_write(FILE3, 7 * i + i * 2, "ABCDEFG", 7);
	}
	f_dump();

	return 0;
}


