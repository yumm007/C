#ifndef __MINI_FS_CONF__
#define __MINI_FS_CONF__

/**< 文件名，使用ID的方式替代字符串 */
typedef enum {
	FILE1,
	
	F_EPD_CFG = FILE1,	//第一个文件，价签配置信息
	F_BMP_CURR,				//当前显示的图片
	F_BMP_BUFF,				//接收到的块信息
	F_BMP_DIFF,				//计算后得到的新图片

	FILE_ID_END,
} file_id_t;

/**< 定义每个文件的大小，使用结构体的方式方便计算文件的起始地址 */
typedef struct FILE_LEN_TABLE {
	BYTE FILE1_SIZE[512];
	BYTE FILE2_SIZE[3096/2 +8];
	BYTE FILE3_SIZE[3096/2 +8];
	BYTE FILE4_SIZE[3096/2 +8];
} FILE_LEN_TABLE;

#define FS_DISK_ROM_FLASH				/**< 使用MCU内部的ROM FLASH，主要区别在读方式上 */
//#define FS_DISK_SPI_FLASH					/**< 使用外部的SPI FLASH */
#define FS_DISK_RAM_FLASH					/**< 使用内存作为文件系统, 主要用于测试 */


#ifdef FS_DISK_ROM_FLASH					/**< 内部FLASH的读写参数 */
#define SEGMENT_SIZE		512				/**< FLASH的最小擦除单元大小 */
#define MAX_WRITE_UNIT	32					/**< FLASH的最大写入单元 */
#endif

#ifdef FS_DISK_SPI_FLASH					/**< 外部FLASH的读写参数 */
#define SEGMENT_SIZE		4096				/**< FLASH的最小擦除单元大小 */
#define MAX_WRITE_UNIT	256				/**< FLASH的最大写入单元 */
#endif

#ifdef FS_DISK_RAM_FLASH					/**< 内部FLASH的读写参数 */
#define SEGMENT_SIZE		64				/**< FLASH的最小擦除单元大小 */
#define MAX_WRITE_UNIT	32					/**< FLASH的最大连续写入单元 */
#endif


#define DISK_BLOCK	13						/**< 使用10个SEGMENT作为文件系统，其中最后2个SEGMENT分别作为超级块和交换块，不能小于3 */
#define SEGMENT_TO_SEGMENT_BUF	32		/**< SPI FLASH需要一个临时空交换块间数据 */

#endif
