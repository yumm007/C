#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "mini_fs.h"

#ifndef FS_USE_MEM_SWAP
#define SWAP_ADDR    ((WORD)SEGMENT_SIZE)*(DISK_BLOCK-1)
#else
#define SWAP_ADDR 	((WORD)&__FS_SWAP_SPACE[0])
#endif


#define SIZEOF(s,m) ((size_t) sizeof(((s *)0)->m)) 

enum {BLOCK_UNUSED, BLOCK_USED, BLOCK_FAIL};

/**< 文件系统结构体 */
typedef struct  {
	BYTE valid;
	BYTE flag;							/**< 文件系统标志位 */
#ifdef ENABLE_BLOCK_MGMT
	BYTE block_status[DISK_BLOCK];/**< 每个物理块的状态 */
	WORD block_map[DISK_BLOCK];	/**< 文件系统使用的物理块序号 */
	WORD block_wc[DISK_BLOCK];		/**< 记录每个块被擦写的次数 */
#endif
	struct file_info_t {
		WORD start_addr;				/**< 文件起始地址 */
		WORD file_len;					/**< 文件当前长度 */
		WORD file_size;				/**< 文件最大长度 */
	} file[FILE_ID_END];
} fs_t;
static fs_t fs = { 
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

#define SUPER_BLOCK    ((WORD)SEGMENT_SIZE)*(DISK_BLOCK -1 - (sizeof(fs_t) + SEGMENT_SIZE -1) / SEGMENT_SIZE)

#define fprintf(...) 
#define printf(...) 

static void disk_clean(WORD addr, WORD len);
static void disk_read(WORD addr, BYTE *data, WORD len);
static void disk_edit(WORD addr, const BYTE *data, WORD len);
static void disk_append(WORD addr, const BYTE *data, WORD len);

/*******************************************************
***	用户接口层代码
***	read函数直接返回FLASH地址即可
*** 	write函数需要进行FLASH擦写函数配合
***	erase函数擦除指定的文件
*******************************************************/

#ifdef FS_DISK_ROM_FLASH
extern const BYTE *f_disk_addr(void);
const BYTE* f_rom_read(file_id_t id, WORD offset, WORD len) {
	if (id >= FILE_ID_END)
		return NULL;
	if (offset + len > fs.file[id].file_size)
		len = fs.file[id].file_size - offset;
	return &f_disk_addr()[fs.file[id].start_addr + offset];
}
#endif

BYTE*	f_read(file_id_t id, WORD offset,	BYTE *buf, WORD len) {
	if (id >= FILE_ID_END || buf == NULL)
		return NULL;
	if (len == 0)
		return buf;
	if (offset + len > fs.file[id].file_size)
		len = fs.file[id].file_size - offset;
	disk_read(fs.file[id].start_addr + offset, buf, len);
	return buf;
}

WORD f_write(file_id_t id,	WORD offset,	const BYTE *data, WORD len) {
	WORD n;

	if (id >= FILE_ID_END || len == 0)
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
	if ( id >= FILE_ID_END)
		return;
	disk_clean(fs.file[id].start_addr, fs.file[id].file_len);
	fs.file[id].file_len = 0;
}

WORD f_len(file_id_t id) {
	if ( id >= FILE_ID_END)
		return 0;
	return fs.file[id].file_len;
}

WORD	f_size(file_id_t id) {
	if ( id >= FILE_ID_END)
		return 0;
	return fs.file[id].file_size;
}

WORD	f_addr(file_id_t id) {
	if ( id >= FILE_ID_END)
		return 0;
	return fs.file[id].start_addr;
}

void f_sync(void) {
	int i;
	disk_edit(SUPER_BLOCK, (BYTE *)&fs, sizeof(fs));
	for (i = 0; i < DISK_BLOCK; i++) {
		if (fs.block_wc[i] != 0)
			fprintf(stderr, "block %d erase %lu times\n", i, fs.block_wc[i]);
	}
}

void f_init(void) {
	file_id_t id;
	BYTE p;
	segment_read(SUPER_BLOCK, 0, (WORD)&p, sizeof(fs.valid));
	
	printf("FS use %d bytes, ", sizeof(fs));
	if (p != 0x76) {
#ifdef ENABLE_BLOCK_MGMT
	int n;
	for (n = 0; n < DISK_BLOCK; n++) {
		fs.block_map[n] = n; //初始化时逻辑块和物理块是联系对应的
		fs.block_status[n] = BLOCK_UNUSED;
		fs.block_wc[n] = 0;
	}
#endif
		for (id = FILE1; id < FILE_ID_END; id++) {
			fs.file[id].file_len = fs.file[id].file_size;
			f_erase(id);
		}
		fs.valid = 0x76;
		f_sync();
		printf("FS init again.\n");
	} else {
		segment_read(SUPER_BLOCK, 0, (WORD)&fs, sizeof(fs));
		printf("FS Load.\n");
	}
}

/*******************************************************
***	块层代码
***	主要涉及到块对齐和块擦除时复制已有数据区的操作
*******************************************************/

//对齐操作集
typedef void (*op_fun_t)(WORD seg_addr, WORD offset, WORD data, WORD len);
static void __addr_split_opera(WORD addr, WORD data, WORD len, WORD split_unit, op_fun_t op) {
	int i, temp_off, temp_len;
	//第一步，处理未对其的部分
	if ((temp_off = addr % split_unit) != 0) {
		temp_len = temp_off + len > split_unit ? split_unit - temp_off : len;
		op(addr - temp_off, temp_off, data, temp_len);
		addr += temp_len; data += temp_len; len -= temp_len;
	}
	//第二步, 处理刚好对齐的部分
	for (i = len / split_unit; i > 0; i--, addr += split_unit, data += split_unit, len -= split_unit)
		op(addr, 0, data, split_unit);
	//第三步,处理最后剩下的
	if (len != 0)
		op(addr, 0, data, len);
}

//块操作函数虚拟映射
static void __segment_erase(WORD addr);
static void __segment_read(WORD seg_addr, WORD seg_off, WORD buf, WORD len);
static void __segment_write(WORD seg_addr, WORD seg_off,  WORD buf, WORD len);

/************************************************
*****************块间复制函数********************
************************************************/

#ifdef ENABLE_BLOCK_MGMT
static WORD get_phy_addr(WORD virt_addr);
#else
#define get_phy_addr(virt_addr) (virt_addr)
#endif

#ifndef FS_USE_MEM_SWAP	//不使用内存交换块间数据

#define SEGMENT_TO_SWAP	segment_copy_segment
#define SWAP_TO_SEGMENT	segment_copy_segment

#if defined(FS_DISK_ROM_FLASH) || defined(FS_DISK_RAM_FLASH)
void segment_copy_segment(WORD seg_dst, WORD dst_off, WORD seg_src, WORD len) {
	extern const BYTE *DISK;
	//fprintf(stderr, "%s(%lu, %lu, %lu, %lu)\n", __FUNCTION__, get_phy_addr(seg_dst), dst_off, get_phy_addr(seg_src), len);
	__segment_write(seg_dst, dst_off, (WORD)&DISK[get_phy_addr(seg_src)], len);
}
#endif

#ifdef FS_DISK_SPI_FLASH
void segment_copy_segment(WORD seg_dst, WORD dst_off, WORD seg_src, WORD len) {
	WORD dst_addr = FS_DISK_ADDR + seg_dst + dst_off;
	WORD src_addr = FS_DISK_ADDR + seg_src;
	BYTE buf[SEGMENT_TO_SEGMENT_BUF];	//SPI FLASH需要一个临时空交换块间数据
	BYTE i;

	for (i = len / SEGMENT_TO_SEGMENT_BUF; i > 0; i--, dst_addr += SEGMENT_TO_SEGMENT_BUF, \
			src_addr += SEGMENT_TO_SEGMENT_BUF, len -= SEGMENT_TO_SEGMENT_BUF) 
	{
		__segment_read(src_addr, 0, (WORD)buf, SEGMENT_TO_SEGMENT_BUF);
		__segment_write(dst_addr, 0, (WORD)buf, SEGMENT_TO_SEGMENT_BUF);
	}
	
	if (len != 0) {
		__segment_read(src_addr, 0, (WORD)buf, len);
		__segment_write(dst_addr, 0, (WORD)buf, len);
	}
}
#endif

#else	//使用内存作为块间数据交换时

#define SEGMENT_TO_SWAP	segment_to_mem 
#define SWAP_TO_SEGMENT	__segment_write
static void segment_to_mem(WORD mem_addr, WORD mem_offset, WORD flash_addr, WORD len){
	WORD seg_addr = flash_addr / SEGMENT_SIZE * SEGMENT_SIZE;
	__segment_read(seg_addr , flash_addr - seg_addr, mem_addr + mem_offset, len);
}
#endif


#define is_contain(a, b, c,d ) ((d) >= (a) && (c) < (b))
#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

static void __segment_op(WORD seg_addr, WORD a, WORD b, BYTE step) {
	WORD id, c, d, min, max;

	for (id = FILE1; id < FILE_ID_END; id++) {
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
			if (step == 0)	//将数据从FLASH区C拷贝到缓冲区SWAP
				__addr_split_opera(SWAP_ADDR + c - seg_addr, c, min - c, MAX_WRITE_UNIT, SEGMENT_TO_SWAP);
			else				//将数据从缓冲区拷贝到FLASH
				__addr_split_opera(c, SWAP_ADDR + c - seg_addr, min - c, MAX_WRITE_UNIT, SWAP_TO_SEGMENT);
		}
		if (d > max) {
			//fprintf(stderr, "area 2: %d, %d, %d, %d\n", a, b, c, d);
			if (step == 0)
				__addr_split_opera(SWAP_ADDR + max - seg_addr, max, d - max, MAX_WRITE_UNIT, SEGMENT_TO_SWAP);
			else
				__addr_split_opera(max, SWAP_ADDR + max - seg_addr, d - max, MAX_WRITE_UNIT, SWAP_TO_SEGMENT);
		}
	}
}

//擦除指定地址的指定长度
static void segment_clean(WORD seg_addr, WORD offset, WORD noused, WORD len) {
	//fprintf(stderr, "%s(%d,%d,,%d)\n", __FUNCTION__, seg_addr, offset, len);
#ifndef FS_USE_MEM_SWAP
	__segment_erase(SWAP_ADDR);
#endif
	__segment_op(seg_addr, seg_addr + offset, seg_addr + offset + len, 0);
	__segment_erase(seg_addr);
	__segment_op(seg_addr, seg_addr + offset, seg_addr + offset + len, 1);
}

static void segment_edit(WORD seg_addr, WORD offset, WORD data, WORD len) {
	if (len == 0)
		return;
	segment_clean(seg_addr, offset, data, len);	
	//写入用户数据
	__addr_split_opera(seg_addr + offset, (WORD)data, len, MAX_WRITE_UNIT, __segment_write);
}

static void disk_edit(WORD addr, const BYTE *data, WORD len) {
	//segment_edit 先擦除再写，数据源一部分是FLASH，一部分是内存
	__addr_split_opera(addr, (WORD)data, len, SEGMENT_SIZE, segment_edit);
}

static void disk_append(WORD addr, const BYTE *data, WORD len) {
	//segment_write 只需要写，数据源只会是内存，已经被擦除好了
	__addr_split_opera(addr, (WORD)data, len, MAX_WRITE_UNIT, __segment_write);
}

static void disk_clean(WORD addr, WORD len) {
	//segment_clean 只负责擦, 不需要数据源
	__addr_split_opera(addr, (WORD)NULL, len, SEGMENT_SIZE, segment_clean);
}

static void disk_read(WORD addr, BYTE *buf, WORD len) {
	//segment_clean 只负责读, 理论上没有对齐限制
	__addr_split_opera(addr, (WORD)buf, len, SEGMENT_SIZE, (op_fun_t)__segment_read);
}


/*******************************************************
***	坏块管理单元
***	文件系统使用连续的地址，但物理上这些地址可能不是连续的
*******************************************************/
#ifdef ENABLE_BLOCK_MGMT
//逻辑地址到物理地址的转换
//block_map存储的是文件系统使用的物理块的顺序，
//文件系统使用的块地址是连续的，但这些块在物理上可能就不是连续的，
//文件系统使用的块0可能是物理块n。
//(virt_addr) / SEGMENT_SIZE 得到文件系统块编号n，而这个编号的实际物理块是block_map[n]
static WORD get_phy_block(WORD virt_addr) {
	WORD phy_block = fs.block_map[(virt_addr) / SEGMENT_SIZE];
	//fprintf(stderr, "phy_block = %lu, virt_addr = %lu\n", phy_block, virt_addr);
	return phy_block;
}
static WORD get_phy_addr(WORD virt_addr) { 
	WORD phy_addr = get_phy_block(virt_addr) * SEGMENT_SIZE + (virt_addr) % SEGMENT_SIZE;
	//fprintf(stderr, "virt_addr %lu -> phy_addr %lu \n", virt_addr, phy_addr);
	return phy_addr;
}
//查找空闲块，返回-1则说明没有空闲块
static int get_free_block(void) {
	int i;
	for (i = 0; i < DISK_BLOCK; i++)
		if (fs.block_status[i] == BLOCK_UNUSED) {
			fs.block_status[i] = BLOCK_USED;
			return i;
		}
	return -1;
}

static void __segment_erase(WORD addr) {
	int phy_block = get_phy_block(addr);
	while (fs.block_wc[phy_block] >= BLOCK_ERASE_MAX || segment_erase(get_phy_addr(addr)) != true) {
		//fprintf(stderr, "block %u failed, ", phy_block);
		fs.block_status[phy_block] = BLOCK_FAIL;	//将此物理块标为坏块
		if ((phy_block = get_free_block()) == -1) {
			//return;	//todo 得不到新块如何处理
			fprintf(stderr, "----no free block----\n");
			exit(1);
		}
		//fprintf(stderr, "request new block %d for it.\n", phy_block);
		fs.block_map[addr / SEGMENT_SIZE] = phy_block;	//更新块映射
	}
	fs.block_wc[phy_block]++;
	fs.block_status[phy_block] = BLOCK_USED;		//成功时也强制更新下，主要是为了init时更新每个块的状态
}
static void __segment_read(WORD seg_addr, WORD seg_off, WORD buf, WORD len) {
	while (segment_read(get_phy_addr(seg_addr), seg_off, buf, len) != true) {
		//fprintf(stderr, "disk failed\n");
		;
	}
}
static void __segment_write(WORD seg_addr, WORD seg_off,  WORD buf, WORD len) {
	int phy_block =  get_phy_block(seg_addr);
	while (segment_write(get_phy_addr(seg_addr), seg_off, buf, len) != true) {
		//fprintf(stderr, "disk failed\n");
		//先将已有数据copy到交换分区, BUG：交换分区此时可能已经在使用
		#ifndef FS_USE_MEM_SWAP
		__segment_erase(SWAP_ADDR);
		#endif
		__segment_op(seg_addr, seg_addr + seg_off, seg_addr + seg_off + len, 0);
		
		//获得新的存储块
		fs.block_status[phy_block] = BLOCK_FAIL;	//将此物理块标为坏块
		__segment_op(seg_addr, seg_addr + seg_off, seg_addr + seg_off + len, 0);
		if ((phy_block = get_free_block()) == -1)
			return;	//todo 得不到新块如何处理
		fs.block_map[seg_addr / SEGMENT_SIZE] = phy_block;	//更新块映射
		
		//擦除新块, 并将原块中的其他数据copy到新块中。BUG: 擦除失败时可能会引发递归循环
		__segment_erase(seg_addr);
		__segment_op(seg_addr, seg_addr + seg_off, seg_addr + seg_off + len, 1);
			
	}
	fs.block_status[phy_block] = BLOCK_USED;		//成功时也强制更新下，主要是为了init时更新每个块的状态 
}
#else
static void __segment_erase(WORD addr) {
	segment_erase(addr);
}
static void __segment_read(WORD seg_addr, WORD seg_off, WORD buf, WORD len) {
	segment_read(seg_addr, seg_off, buf, len);
}
static void __segment_write(WORD seg_addr, WORD seg_off,  WORD buf, WORD len) {
	segment_write(seg_addr, seg_off, buf, len);
}
#endif

