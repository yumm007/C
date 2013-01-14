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
static UINT8 DISK_MAP[DISK_SPACE];	//用于跟踪DISK某个字节所在的区域是否被擦除过


UINT8*	f_read(file_id_t id, 	UINT16 offset,	UINT16 len);
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
UINT8* f_read(file_id_t id, UINT16 offset, UINT16 len) {
	if (id < FILE_ID_BEGIN || id > FILE_ID_END)
		return NULL;
	if (offset + len > file_info[id].file_size)
		len = file_info[id].file_size - offset;
	return &DISK[file_info[id].start_addr + offset];
}

void f_write(file_id_t id,	UINT16 offset,	const UINT8 *data, UINT16 len) {
	UINT16 n;

	if (id < FILE_ID_BEGIN || id > FILE_ID_END)
		return;
	if (offset + len > file_info[id].file_size)
		len = file_info[id].file_size - offset;
	
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
	disk_clean(file_info[id].start_addr, file_info[id].file_size);
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
	//fprintf(stderr, "%s(%d)\n", __FUNCTION__, addr);
	if (addr % SEGMENT_SIZE != 0)
		fprintf(stderr, "%s:segment\n", __FUNCTION__);
	memset(&DISK[addr], '0', SEGMENT_SIZE);
	memset(&DISK_MAP[addr], 1, SEGMENT_SIZE);
}

static int erase_byte_count(UINT16 addr, UINT16 len) {
	int i, ret = 0;
	for (i = 0; i < len; i++)
		ret += DISK_MAP[addr + i];
	//fprintf(stderr, "%s(%d,%d) = %d\n", __FUNCTION__, addr, len, ret);
	return ret;
}

//需要移植的函数，将数据从内存写到FLASH，调用者保证所在的FLASH已经被擦过且操作不跨段
static void segment_copy_mem(UINT16 addr, UINT16 offset,  const UINT8 *data, UINT16 len) {
	//fprintf(stderr, "%s(%d, %d, %p, %d)\n", __FUNCTION__, addr, offset, data, len);
	if (offset + len > SEGMENT_SIZE || addr % SEGMENT_SIZE != 0)
		fprintf(stderr, "%s:segment not split\n", __FUNCTION__);
	if (erase_byte_count(addr + offset, len) != len)
		fprintf(stderr, "%s:write before erase:%d+%d, %d\n", __FUNCTION__, \
				addr, offset, len);
	memcpy(&DISK[addr + offset], data, len);
	memset(&DISK_MAP[addr + offset], 0, len);
}

//需要移植的函数，将数据从FLASH拷贝到FLASH，调用者保证目标所在FLASH已经被擦除且操作不夸段
static void segment_copy_segment(UINT16 des, UINT16 src, UINT16 len) {
	//fprintf(stderr, "%s(%d, %d, %d)\n", __FUNCTION__, des, src, len);
	if (des % SEGMENT_SIZE + len > SEGMENT_SIZE)
		fprintf(stderr, "%s:segment not split\n", __FUNCTION__);
	if (src % SEGMENT_SIZE + len > SEGMENT_SIZE)
		fprintf(stderr, "%s:segment not split\n", __FUNCTION__);
	if (erase_byte_count(des, len) != len)
		fprintf(stderr, "%s:write before erase:%d, %d\n", __FUNCTION__, des, len);
	memcpy(&DISK[des], &DISK[src], len);
	memset(&DISK_MAP[des], 0, len);
}

#if 0
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
#else
static void segment_clean(UINT16 addr, UINT16 offset, const UINT8 *noused, UINT16 len) {
	//1，先擦除SWAP
	//2，遍历文件数组，判断某个文件是否和此段有交集
	//3, 如果有,计算出此交集和区域addr+offset - addr+offset+len的交集部分，
	//4, 拷贝到SWAP
	//5，擦除addr，重复2-4，源和目标地址相反，

	int id;
	UINT16 s1, len1, s, l;
	
	//fprintf(stderr, "%s(%d,%d,,%d)\n", __FUNCTION__, addr, offset, len);

	segment_erase(SWAP_ADDR);
	for (id = FILE1; id < FILE_ID_END; id++) {
		s1 = file_info[id].start_addr;
		len1 = file_info[id].file_len;
		//fprintf(stderr, "cmp FILE(%d): start %d, len %d\n", id, s1, len1);
		//计算出文件和段的交集部分
		if (s1 < addr && (s1 + len1) > addr && (s1 + len1) < (addr + SEGMENT_SIZE)) {
			//文件尾部在此段中
			s = addr; l = s1 + len - addr;
			//fprintf(stderr, "include tail s %d l %d\n", s, l);
		} else if (s1 >= addr && s1 + len1 > addr + SEGMENT_SIZE) {
			//文件头部在此段中
			s = s1; l = addr + SEGMENT_SIZE - s1;
			//fprintf(stderr, "include head s %d l %d\n", s, l);
		} else if ( s1 >= addr && s1 + len1 <= addr + SEGMENT_SIZE) {
			//文件全部在此段中
			s = s1, l = len1;
			//fprintf(stderr, "include body s %d l %d\n", s, l);
		} else
			continue;
		//计算此交集和擦写区域的交集, 跳过交集部分
		if (s + l < offset || offset + len < s)
			continue;
		//if (addr + offset > s)
		//	s = addr + offset;
		//if (addr + len < l)
		//	l = addr + len;
		//segment_copy_segment(SWAP_ADDR + s - addr, s, l);
		if (s < addr + offset)
			segment_copy_segment(SWAP_ADDR + s - addr, s, addr + offset - s);
		if (addr + offset + len < s + l)
			segment_copy_segment(SWAP_ADDR + addr + offset + len, \
						addr + offset + len , s + l - (addr + offset + len));
			
	}

	segment_erase(addr);
	for (id = FILE1; id < FILE_ID_END; id++) {
		s1 = file_info[id].start_addr;
		len1 = file_info[id].file_len;
		if (s1 < addr && (s1 + len1) > addr && (s1 + len1) < (addr + SEGMENT_SIZE)) {
			//文件尾部在此段中
			s = addr; l = s1 + len - addr;
		} else if (s1 >= addr && s1 + len1 > addr + SEGMENT_SIZE) {
			//文件头部在此段中
			s = s1; l = addr + SEGMENT_SIZE - s1;
		} else if ( s1 >= addr &&s1 + len1 <= addr + SEGMENT_SIZE) {
			//文件全部在此段中
			s = addr; l = len1;
		} else
			continue;
		//计算此交集和擦写区域的交集
		if (s + l < offset || offset + len < s)
			continue;
		//if (addr + offset > s)
		//	s = addr + offset;
		//if (addr + len < l)
		//	l = addr + len;
		//segment_copy_segment(s, SWAP_ADDR + s - addr, l);
		if (s < addr + offset)
			segment_copy_segment(s, SWAP_ADDR + s - addr, addr + offset - s);
		if (addr + offset + len < s + l)
			segment_copy_segment(addr + offset + len, \
						SWAP_ADDR + addr + offset + len, s + l - (addr + offset + len));
	}
}

#endif
static void segment_write(UINT16 addr, UINT16 offset, const UINT8 *data, UINT16 len) {
	if (len == 0)
		return;
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
	if (temp_len != 0) {
		//fprintf(stderr, "step 1:\n");
		op(temp_addr, temp_off, data, temp_len);
	}
	
	//第二步, copy 刚好对齐的部分
	offset = temp_addr + SEGMENT_SIZE;
	data += temp_len;
	len -= temp_len;
	
	n = len / SEGMENT_SIZE;
	//printf("block:%d / %d")
	for (i = 0; i < n; i++) {
		//fprintf(stderr, "step 2:\n");
		op(offset + i * SEGMENT_SIZE, 0, data + i * SEGMENT_SIZE, SEGMENT_SIZE);
	}

	//第三步
	offset += n * SEGMENT_SIZE;
	data += n * SEGMENT_SIZE;
	len -= n * SEGMENT_SIZE;
	if (len != 0) {
		//fprintf(stderr, "step 3:\n");
		op(offset, 0, data, len);
	}
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

void f_test(void) {
	UINT8 tmp_data[20] = "this is a test line";
	//UINT8 tmp_read[20];

	//erase 测试
	f_erase(FILE1);
	fprintf(stderr, "f_erase(FILE1) Finished.\n");
	f_erase(FILE2);
	fprintf(stderr, "f_erase(FILE2) Finished.\n");
	f_erase(FILE3);
	fprintf(stderr, "f_erase(FILE3) Finished.\n");


	f_write(FILE1, 2, tmp_data, 5);
	fprintf(stderr, "f_write(FILE1) Finished.\n");
	if (memcmp(f_read(FILE1, 2, 5), tmp_data, 5) != 0)
		fprintf(stderr, "%s:%d erase failed.\n", __FUNCTION__, __LINE__);
	//读写测试
	fprintf(stderr, "f_write(FILE1) Begin.\n");
	f_write(FILE1, 2, tmp_data, 6);
	fprintf(stderr, "f_write(FILE1) Finished.\n");
	if (memcmp(f_read(FILE1, 2, 6), tmp_data, 6) != 0)
		fprintf(stderr, "%s:%d f_write failed.\n", __FUNCTION__, __LINE__);

	//连续写测试
	f_write(FILE2, 0, tmp_data, 15);
	f_write(FILE1, 9, tmp_data, 1);
	f_write(FILE2, 10, tmp_data, 10);
	if (memcmp(f_read(FILE2, 10, 10), tmp_data, 10) != 0) 
		fprintf(stderr, "%s:%d more write failed.\n", __FUNCTION__, __LINE__);
	//跨区写测试

	//
}


int main(void) {
	UINT8 i;
	UINT8 tmp[17] = "this is a test";

	//memset(DISK_MAP, 1, sizeof(DISK_MAP));
	f_test();
	memset(DISK, '0', sizeof(DISK));
	for (i = 0; i < 10; i++) {
		f_write(FILE2, 1, tmp, sizeof(tmp));
		f_write(FILE3, 20, (UINT8 *)"abcdefg", 7);
		f_write(FILE3, 35, (UINT8 *)"1234567", 7);
		f_write(FILE3, 2, (UINT8 *)"ABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFGABCDEFG", 63);
		f_write(FILE3, 14, (UINT8 *)"ZZZ", 3);
		f_write(FILE3, 14, (UINT8 *)"DDDDDDDDDDDDDDDDDDD", 19);
		f_write(FILE1, 2, (UINT8 *)"ABCDEFGABC", 8);
		f_write(FILE3, 7 * i + i * 2, (UINT8 *)"ABCDEFG", 7);
	}
	

	f_dump();
	return 0;
}


