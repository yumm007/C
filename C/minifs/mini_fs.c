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
	UINT16		file_size;
};

static struct file_info_t file_info[] = {
	{FILE1, 0, 0, 10},
	{FILE2, 10, 0, 23},
	{FILE3, 33, 0, 150},
};

#define SEGMENT_SIZE	16
#define DISK_SPACE	SEGMENT_SIZE*50
#define SWAP_ADDR	SEGMENT_SIZE*49
static UINT8 DISK[DISK_SPACE];


UINT8 *	f_read(file_id_t id, 	UINT16 offset,	UINT16 len);
void 		f_write(file_id_t id, 	UINT16 offset,	const UINT8 *data, UINT16 len);
void		f_erase(file_id_t id);
static void disk_edit(UINT16 offset, const UINT8 *data, UINT16 len);
static void disk_append(UINT16 offset, const UINT8 *data, UINT16 len);
static void disk_clean(UINT16 offset, UINT16 len);

/*******************************************************
***	用户接口层代码
***	read函数直接返回FLASH地址即可
*** 	write函数需要进行FLASH擦写函数配合
***	erase函数擦除指定的文件
*******************************************************/
UINT8 * f_read(file_id_t id, UINT16 offset, UINT16 len) {
	if (id < FILE_ID_BEGIN || id > FILE_ID_END)
		return NULL;
	if (offset + len > file_info[id].file_size)
		return NULL;
	return (UINT8 *)(file_info[id].start_addr + offset);
}

void f_write(file_id_t id,	UINT16 offset,	const UINT8 *data, UINT16 len) {
	UINT16 n;

	if (id < FILE_ID_BEGIN || id > FILE_ID_END)
		return;
	if (offset + len > file_info[id].file_size)
		return;
	
	//每次写文件时，计算出需要修改的部分和需要追加的部分
	//需要修改的部分设计到擦除FLASH，而追加的部分则在文件创建时已经擦写过了

	if (offset >= file_info[id].file_len) {
		//需要保存的数据只需要追加
		disk_append(file_info[id].start_addr + offset, data, len);
		file_info[id].file_len = offset + len;
	} else if (offset < file_info[id].file_len && offset + len > file_info[id].file_len) {
		//需要保存的数据一部分位于已有数据内部，另外一部分需要追加
		n = file_info[id].file_len - offset;
		disk_edit(file_info[id].start_addr + offset, data, n);
		disk_append(file_info[id].start_addr + file_info[id].file_len, &data[n], len - n);
		file_info[id].file_len = offset + len;
	} else {
		//需要保存的数据完全位于已有数据内部
		disk_edit(file_info[id].start_addr + offset, data, len);
	}
}

void f_erase(file_id_t id) {
	if (id < FILE_ID_BEGIN || id > FILE_ID_END)
		return;
	disk_clean(file_info[id].start_addr, file_info[id].file_len);
	file_info[id].file_len = 0;
}

static void f_dump(void) {
	int i, j, n;
	n = sizeof(file_info) / sizeof(file_info[0]);
	for (i = 0; i < n; i++) {
		printf("FILE %d: addr = %d, len = %d, size = %d\n", i, \
			file_info[i].start_addr, file_info[i].file_len, file_info[i].file_size);
		for (j = 0; j < file_info[i].file_size; j++)
			putchar(DISK[file_info[i].start_addr + j]);
		putchar('\n');
	}
}

/*******************************************************
***	底层IO函数
***	需要实现Flash擦除函数，内存到Flash的复制，
***  flash到flash的复制共3个函数
*******************************************************/

//需要移植的函数, 调用者确保地址已经按segment对齐
static void segment_erase(UINT16 addr) {
	if (addr % SEGMENT_SIZE) {
		fprintf(stderr, "非对齐擦除！\n");
		return;
	}
	memset(&DISK[addr], '0', SEGMENT_SIZE);
}

//需要移植的函数，将数据从内存写到FLASH，调用者保证所在的FLASH已经被擦过了.
static void segment_copy_mem(UINT16 addr, UINT16 offset,  const UINT8 *data, UINT16 len) {
	memcpy(&DISK[addr + offset], data, len);
}

//需要移植的函数，将数据从FLASH拷贝到FLASH，调用者保证目标所在FLASH已经被擦除了.
static void segment_copy_segment(UINT16 des, UINT16 src, UINT16 len) {
	memcpy(&DISK[des], &DISK[src], len);
}

static void segment_clean(UINT16 addr, UINT16 offset, const UINT8 *noused, UINT16 len) {
	UINT16 n = offset + len;

	//擦除缓存，将目标所在块的其余数据搬至此
	segment_erase(SWAP_ADDR);
	segment_copy_segment(SWAP_ADDR, addr, offset); //前
	segment_copy_segment(SWAP_ADDR + n, addr + n, SEGMENT_SIZE - n);	//后

	//擦除目标块，将刚才缓存的数据写回
	segment_erase(addr);
	segment_copy_segment(addr, SWAP_ADDR, offset);	//前
	segment_copy_segment(addr + n, SWAP_ADDR + n, SEGMENT_SIZE - n); //后
}

static void segment_write(UINT16 addr, UINT16 offset, const UINT8 *data, UINT16 len) {
	segment_clean(addr, offset, data, len);	
	segment_copy_mem(addr, offset, data, len); 	//写入用户数据
}

//块对齐操作集
typedef void (*op_fun_t)(UINT16 addr, UINT16 offset, const UINT8 *data, UINT16 len);
static void __addr_split_opera(UINT16 offset, const UINT8 *data, UINT16 len, op_fun_t op) {
	UINT16 temp_addr, temp_off, temp_len;
	int i, n;
	
	//第一步，offset向下对齐 - offset, 知道需要改写的上一块地址
	temp_addr = (offset / SEGMENT_SIZE ) * SEGMENT_SIZE;
	temp_off = offset - temp_addr;
	temp_len = SEGMENT_SIZE - temp_off > len ? len : SEGMENT_SIZE - temp_off;
	
	if (temp_len != 0) 
		op(temp_addr, temp_off, data, temp_len);
	
	//第二步, copy 刚好对齐的部分
	offset = temp_addr + SEGMENT_SIZE;
	data += temp_len;
	len -= temp_len;
	
	n = len / SEGMENT_SIZE;
	//printf("block:%d / %d")
	for (i = 0; i < n; i++) 
		op(offset + i * SEGMENT_SIZE, 0, data + i * SEGMENT_SIZE, SEGMENT_SIZE);
	
	//第三步
	offset += n * SEGMENT_SIZE;
	data += n * SEGMENT_SIZE;
	len -= n * SEGMENT_SIZE;
	if (len != 0) 
		op(offset, 0, data, len);
}

static void disk_edit(UINT16 offset, const UINT8 *data, UINT16 len) {
	//segment_write 先擦除再写，数据源一部分是FLASH，一部分是内存
	__addr_split_opera(offset, data, len, segment_write);
}

static void disk_append(UINT16 offset, const UINT8 *data, UINT16 len) {
	//segment_copy 只需要写，数据源只会是内存，已经被擦除好了
	__addr_split_opera(offset, data, len, segment_copy_mem);
}

static void disk_clean(UINT16 offset, UINT16 len) {
	//segment_clean 只负责擦, 不需要数据源
	__addr_split_opera(offset, NULL, len, segment_clean);
}


/*******************************************************
***	用户层测试函数
*******************************************************/

int main(void) {
	UINT8 i;
	//UINT8 tmp[17] = "this is a test";

	memset(DISK, '0', sizeof(DISK));
	for (i = 0; i < 1; i++) {
		//f_write(FILE2, 1, tmp, sizeof(tmp));
		//f_write(FILE3, 20, (UINT8 *)"abcdefg", 7);
		//f_write(FILE3, 35, (UINT8 *)"1234567", 7);
		//f_write(FILE3, 2, (UINT8 *)"ABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFG", 63);
		//f_write(FILE3, 14, (UINT8 *)"ZZZ", 3);
		//f_dump();
		f_write(FILE3, 14, (UINT8 *)"DDDDDDDDDDDDDDDDDDD", 19);
		f_dump();
		//f_write(FILE1, 2, (UINT8 *)"ABCDEFGABC", 8);
		//f_write(FILE3, 7 * i + i * 2, (UINT8 *)"ABCDEFG", 7);
	}

	return 0;
}


