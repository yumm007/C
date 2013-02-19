/**
Copyright (C) 2012, 北京汉朔科技有限公司
All Rights Reserved World Wide.
Project：		ESL
Description:	FLASH文件系统
Date			Author			Description
2013-01-31	于明明			beta版本
*/

#ifndef __MINI_FS__
#define __MINI_FS__

/** @defgroup mini_fs
*  文件系统函数说明
*  @{
*/

/**< 基本数据类型定义头文件 */
typedef unsigned char BYTE;

#include "mini_fs_conf.h"
#ifdef FS_DISK_RAM_FLASH
typedef unsigned long WORD;
#else
typedef unsigned int WORD;
#endif

/**< 文件系统配置信息，由平台自定义 */

/**< 文件系统结构体 */
typedef struct fs_t {
	BYTE valid;
	BYTE flag;
	struct file_info_t {
		WORD start_addr;
		WORD file_len;
		WORD file_size;		
	} file[FILE_ID_END];
} fs_t;
extern fs_t fs;

/**< 以下4个宏不需要改动 */
#define FS_BLOCK     ((sizeof(FILE_LEN_TABLE) + SEGMENT_SIZE -1) / SEGMENT_SIZE) 
#define SUPER_BLOCK  (FS_BLOCK + (sizeof(fs) + SEGMENT_SIZE -1) / SEGMENT_SIZE)       
#define SWAP_BLOCK   1  
#define DISK_BLOCK   (FS_BLOCK + SUPER_BLOCK + SWAP_BLOCK)

/**< 系统启动时加载文件系统 */
void 	f_init(void);

/**< 系统关机时或需要时保存文件系统 */
void 	f_sync(void);
#ifdef FS_DISK_ROM_FLASH
const BYTE* f_rom_read(file_id_t id, WORD offset);	/**< 快速读函数，直接返回FLASH地址。只能在ROM FLASH使用 */
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

/**
* 文件拷贝函数
* @param[in] dst 需要写入的文件ID   
* @param[in] dst_offset 从文件的偏移地址dst_offset开始写   
* @param[in] src 待读出的文件ID
* @param[in] src_offset 从文件的偏移地址src_offset开始读 
* @param[in] len 需要拷贝的字节数
* @return 返回成功拷贝的字节数
*/
WORD f_copy(file_id_t dst, WORD dst_offset, file_id_t src, WORD src_offset, WORD len);

/**< 返回文件长度 */
WORD	f_len(file_id_t id);

/**< 返回文件大小 */
WORD	f_size(file_id_t id);

/**< 返回文件地址 */
WORD	f_addr(file_id_t id);

/**< 清空文件 */
void	f_erase(file_id_t id);

/**< 擦除一个最小的块，调用则保证地址addr按 SEGMENT_SIZE 对齐 */
extern void segment_erase(WORD addr);

/**
* IO块读函数
* @param[in] seg_addr 需要读的块起始地址
* @param[in] seg_off 块内偏移量
* @param[in] buf 写入的内存区，参数应该为指针，但传参时需要强转为WORD类型
* @param[in] len 需要读取的字节数, 调用者保证不会超出buf空间
* @return 无
*/
extern void segment_read(WORD seg_addr, WORD seg_off, WORD buf, WORD len);

/**
* IO块写函数
* @param[in] seg_addr 需要写的块起始地址
* @param[in] seg_off 块内偏移量
* @param[in] buf 为数据源，参数应该为指针，但传参时需要强转为WORD类型
* @param[in] len 需要写入的字节数，调用者保证不会跨MAX_WRITE_UNIT写入
* @return 无
*/
extern void segment_write(WORD seg_addr, WORD seg_off,  WORD buf, WORD len);

/**@}*/ // mini_fs

#endif
