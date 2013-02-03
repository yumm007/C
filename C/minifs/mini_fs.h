
#ifndef __MINI_FS__
#define __MINI_FS__

/** @defgroup mini_fs
*  文件系统函数说明
*  @{
*/

/**< 基本数据类型定义头文件 */
typedef unsigned char BYTE;
typedef unsigned long WORD;
#include <stdbool.h>
//#include "datatype.h"

//#define FS_DISK_ROM_FLASH					/**< 使用MCU内部的ROM FLASH，主要区别在读方式上 。内部FLASH读时直接返回FLASH地址*/
//#define FS_DISK_SPI_FLASH				/**< 使用外部的SPI FLASH */
#define FS_DISK_RAM_FLASH					/**< 使用内存作为文件系统, 主要用于测试 */
//#define FS_USE_MEM_SWAP	
//#define ENABLE_BLOCK_MGMT					/**< 启用坏块管理单元 */

#ifdef FS_DISK_ROM_FLASH					/**< 内部FLASH的读写参数 */
#define SEGMENT_SIZE		512				/**< FLASH的最小擦除单元大小 */
#define MAX_WRITE_UNIT	32					/**< FLASH的最大连续写入单元 */
#endif

#ifdef FS_DISK_SPI_FLASH					/**< 外部FLASH的读写参数 */

#define SEGMENT_SIZE		4096				/**< FLASH的最小擦除单元大小 */
#define MAX_WRITE_UNIT	256				/**< FLASH的最大连续写入单元 */
#define FS_DISK_ADDR	0x00					/**< 文件系统在FLASH中的起始位置 */
#endif

#ifdef FS_DISK_RAM_FLASH					/**< 内部FLASH的读写参数 */
#define SEGMENT_SIZE		64				/**< FLASH的最小擦除单元大小 */
#define MAX_WRITE_UNIT	32					/**< FLASH的最大连续写入单元 */
#endif

/**< 使用内存作为块间数据交换空间, 否则在FLASH内开辟一个独立的segment作为交换空间。
*    RAM空间够的话推荐启用这个宏, 这将大大增加文件系统速度，单需要给文件系统指定一个SEGMENT_SIZE字节的BUF区 
*/
					
#ifndef FS_USE_MEM_SWAP
#define SEGMENT_TO_SEGMENT_BUF	32		/**< 如果不使用内存作为块交互区，则定义一个32字节的函数内临时数组 */
#else
extern BYTE __FS_SWAP_SPACE[SEGMENT_SIZE];	/**< 需要用户在程序中指定这个交换区的存储位置 */
#endif

/**< 文件名，使用ID的方式替代字符串 */
typedef enum {
	FILE1,
	FILE2,
	FILE3,
	FILE4,
	FILE5,
	FILE6,
	FILE7,
	FILE8,
	FILE9,

	FILE_ID_END,
} file_id_t;

/**< 定义每个文件的大小，使用结构体的方式方便计算文件的起始地址 */
typedef struct FILE_LEN_TABLE {
	BYTE FILE1_SIZE[10];
	BYTE FILE2_SIZE[23];
	BYTE FILE3_SIZE[150];
	BYTE FILE4_SIZE[9];
	BYTE FILE5_SIZE[43];
	BYTE FILE6_SIZE[120];
	BYTE FILE7_SIZE[10];
	BYTE FILE8_SIZE[300];
	BYTE FILE9_SIZE[600];
} FILE_LEN_TABLE;

/**< 使用足够的SEGMENT作为文件系统，其中最后2个SEGMENT分别作为超级块和交换块，不能小于3 */
#define DISK_BLOCK   ((sizeof(FILE_LEN_TABLE) + SEGMENT_SIZE -1) / SEGMENT_SIZE + 2)

/**< 系统启动时加载文件系统 */
void 	f_init(void);

/**< 系统关机时或需要时保存文件系统 */
void 	f_sync(void);
#ifdef FS_DISK_ROM_FLASH
const BYTE* f_rom_read(file_id_t id, WORD offset, WORD len);	/**< 快速读函数，直接返回FLASH地址。只能在ROM FLASH使用 */
#endif

/**
* 标准读函数 调用者需确认buf空间足够容纳len个字节
* @param[in] id 文件ID   
* @param[in] offset 从文件的偏移地址offset开始读   
* @param[out] buf 将数据读入到buf中
* @param[in] len 读入的字节数
* @return 如果读成功，返回buf地址，如果失败返回NULL
*/
BYTE*	f_read(file_id_t id, WORD offset,	BYTE *buf, WORD len);

/**
* 标准写函数
* @param[in] id 文件ID   
* @param[in] offset 从文件的偏移地址offset开始写   
* @param[in] data 待写入的数据
* @param[in] len 需要写入的字节数
* @return 返回写入成功的字节数
*/
WORD 	f_write(file_id_t id, WORD offset,	const BYTE *data, WORD len);

/**< 返回文件长度 */
WORD	f_len(file_id_t id);

/**< 返回文件大小 */
WORD	f_size(file_id_t id);

/**< 清空文件 */
void	f_erase(file_id_t id);

/**< 擦除一个最小的块，调用则保证地址addr按 SEGMENT_SIZE 对齐 */
extern bool segment_erase(WORD addr);

/**
* IO块读函数
* @param[in] seg_addr 需要读的块起始地址
* @param[in] seg_off 块内偏移量
* @param[in] buf 写入的内存区，参数应该为指针，但传参时需要强转为WORD类型
* @param[in] len 需要读取的字节数, 调用者保证不会超出buf空间
* @return 无
*/
extern bool segment_read(WORD seg_addr, WORD seg_off, WORD buf, WORD len);

/**
* IO块写函数
* @param[in] seg_addr 需要写的块起始地址
* @param[in] seg_off 块内偏移量
* @param[in] buf 为数据源，参数应该为指针，但传参时需要强转为WORD类型
* @param[in] len 需要写入的字节数，调用者保证不会跨MAX_WRITE_UNIT写入
* @return 返回写成功或者失败
*/
extern bool segment_write(WORD seg_addr, WORD seg_off,  WORD buf, WORD len);

/**@}*/ // mini_fs

#endif
