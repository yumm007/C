#ifndef __MINI_FS_CONF__
#define __MINI_FS_CONF__

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


//#define FS_DISK_ROM_FLASH				/**< 使用MCU内部的ROM FLASH，主要区别在读方式上 */
//#define FS_DISK_SPI_FLASH					/**< 使用外部的SPI FLASH */
#define FS_DISK_RAM_FLASH					/**< 使用内存作为文件系统, 主要用于测试 */

//#define F_COPY_USE_EXT_MEM      			/**< f_copy使用外部缓存 */
#define F_COPY_CACHE_SIZE	32


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


#endif
