#include <stdio.h>
#include <string.h>
#include "mini_fs.h"

#define DISK_SPACE   SEGMENT_SIZE*DISK_BLOCK
#define SWAP_ADDR    SEGMENT_SIZE*(DISK_BLOCK-1)
#define SUPER_BLOCK  SEGMENT_SIZE* (DISK_BLOCK-2-(sizeof(fs) + SEGMENT_SIZE -1) / SEGMENT_SIZE)

fs_t fs = { 
   .flag = 0,
   .file = { 
      {0,   0, 10},
      {0+10,  0, 23},
      {10+23,  0, 150},
      {10+23+150,   0, 9},
      {10+23+150+9,  0, 43},
      {10+23+150+9+43,  0, 130},
      {10+23+150+9+43+130,   0, 300},
      {10+23+150+9+43+130+300,  0, 203},
   },  
};

#define fprintf(...) 

UINT8 DISK[DISK_SPACE];
UINT8 DISK_MAP[DISK_SPACE];

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
	if (id > FILE_ID_END)
		return NULL;
	if (offset + len > fs.file[id].file_size)
		len = fs.file[id].file_size - offset;
	return &DISK[fs.file[id].start_addr + offset];
}

UINT16 f_write(file_id_t id,	UINT16 offset,	const UINT8 *data, UINT16 len) {
	UINT16 n;

	if (id > FILE_ID_END)
		return 0;
	if (offset + len > fs.file[id].file_size)
		len = fs.file[id].file_size - offset;
	
	//每次写文件时，计算出需要修改的部分和需要追加的部分
	//需要修改的部分设计到擦除FLASH，而追加的部分则在文件创建时已经擦写过了

	if (offset >= fs.file[id].file_len) {
		//需要保存的数据只需要追加
		disk_append(fs.file[id].start_addr + offset, data, len);
		fs.file[id].file_len = offset + len;
	} else if (offset < fs.file[id].file_len && offset + len > fs.file[id].file_len) {
		//需要保存的数据一部分位于已有数据内部，另外一部分需要追加
		n = fs.file[id].file_len - offset;
		disk_edit(fs.file[id].start_addr + offset, data, n);
		disk_append(fs.file[id].start_addr + fs.file[id].file_len, &data[n], len - n);
		fs.file[id].file_len = offset + len;
	} else {
		//需要保存的数据完全位于已有数据内部
		disk_edit(fs.file[id].start_addr + offset, data, len);
	}

	return len;
}

void f_erase(file_id_t id) {
	if ( id > FILE_ID_END)
		return;
	disk_clean(fs.file[id].start_addr, fs.file[id].file_len);
	fs.file[id].file_len = 0;
}

UINT16 f_len(file_id_t id) {
	if ( id > FILE_ID_END)
		return 0;
	return fs.file[id].file_len;
}

void f_sync(void) {
	//segment_erase(SUPER_BLOCK);
	//disk_clean(SUPER_BLOCK, SWAP_ADDR - SUPER_BLOCK);
	disk_edit(SUPER_BLOCK, (UINT8 *)&fs, sizeof(fs));
	//segment_copy_mem(SUPER_BLOCK, 0, (UINT8 *) &fs, sizeof(fs));
}

void f_init(void) {
	int id;
	fs_t *p = (void *)&DISK[SUPER_BLOCK];
	if (p->valid != 0x76) {
		for (id = 0; id <= FILE_ID_END; id++) {
			fs.file[id].file_len = fs.file[id].file_size;
			f_erase(id);
		}
		fs.valid = 0x76;
		f_sync();
	} else {
		memcpy(&fs, &DISK[SUPER_BLOCK], sizeof(fs));
	}
}

static void f_dump(void) {
	int i, j, n;
	n = sizeof(fs.file) / sizeof(fs.file[0]);
	for (i = 0; i < n; i++) {
		printf("FILE %d: addr = %d, len = %d, size = %d\n", i + 1, \
			fs.file[i].start_addr, fs.file[i].file_len, fs.file[i].file_size);
		for (j = 0; j < fs.file[i].file_size; j++)
			putchar(DISK[fs.file[i].start_addr + j]);
		putchar('\n');
	}
}

/*******************************************************
***	块层代码
***	主要涉及到块对齐和块擦除时复制已有数据区的操作
*******************************************************/

#define is_contain(a, b, c,d ) ((d) >= (a) && (c) < (b))
#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

static void __segment_op(UINT16 seg_addr, UINT16 a, UINT16 b, UINT8 step) {
	UINT16 id, c, d, min, max;

	for (id = FILE1; id <= FILE_ID_END; id++) {
		c = fs.file[id].start_addr;
		d = c + fs.file[id].file_len;
		if (!is_contain(seg_addr, seg_addr + SEGMENT_SIZE, c, d))	//文件不在此块中	
			continue;
		d =  d > seg_addr + SEGMENT_SIZE ? seg_addr + SEGMENT_SIZE: d;
		c = c < seg_addr ? seg_addr : c;
		//fprintf(stderr, "file %d start %d, len = %d, in this segment %d, step %d\n", id + 1, c, d, seg_addr, step);
		min = MIN(a, d);
		max = MAX(c, b);
		//fprintf(stderr, "a=%d,b=%d,c=%d,d=%d\n",a,b,c,d);
		if (c < min) {
			//fprintf(stderr, "area 1: %d, %d, %d, %d\n", a, b, c, d);
			if (step == 0)
				segment_copy_segment(SWAP_ADDR + c - seg_addr, c, min - c);
			else
				segment_copy_segment(c, SWAP_ADDR + c - seg_addr, min - c);
		}
		if (d > max) {
			//fprintf(stderr, "area 2: %d, %d, %d, %d\n", a, b, c, d);
			if (step == 0)
				segment_copy_segment(SWAP_ADDR + max - seg_addr, max, d - max);
			else
				segment_copy_segment(max, SWAP_ADDR + max - seg_addr, d - max);
		}
	}
}

static void segment_clean(UINT16 seg_addr, UINT16 offset, const UINT8 *noused, UINT16 len) {
	fprintf(stderr, "%s(%d,%d,,%d)\n", __FUNCTION__, seg_addr, offset, len);
	segment_erase(SWAP_ADDR);
	__segment_op(seg_addr, seg_addr + offset, seg_addr + offset + len, 0);
	segment_erase(seg_addr);
	__segment_op(seg_addr, seg_addr + offset, seg_addr + offset + len, 1);
}

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
		fprintf(stderr, "step 1:\n");
		op(temp_addr, temp_off, data, temp_len);
	}
	
	//第二步, copy 刚好对齐的部分
	offset = temp_addr + SEGMENT_SIZE;
	data += temp_len;
	len -= temp_len;
	
	n = len / SEGMENT_SIZE;
	for (i = 0; i < n; i++) {
		fprintf(stderr, "step 2:\n");
		op(offset + i * SEGMENT_SIZE, 0, data + i * SEGMENT_SIZE, SEGMENT_SIZE);
	}

	//第三步
	offset += n * SEGMENT_SIZE;
	data += n * SEGMENT_SIZE;
	len -= n * SEGMENT_SIZE;
	if (len != 0) {
		fprintf(stderr, "step 3:\n");
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

