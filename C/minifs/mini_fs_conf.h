#ifndef __MINI_FS_CONF__
#define __MINI_FS_CONF__

/**< ¿¿¿¿¿¿ID¿¿¿¿¿¿¿¿ */
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


//#define FS_DISK_ROM_FLASH				/**< ¿¿MCU¿¿¿ROM FLASH¿¿¿¿¿¿¿¿¿¿ */
//#define FS_DISK_SPI_FLASH					/**< ¿¿¿¿¿SPI FLASH */
#define FS_DISK_RAM_FLASH					/**< ¿¿¿¿¿¿¿¿¿¿, ¿¿¿¿¿¿ */

#define FS_ENABLE_BLOCK_MGMT
//#define FS_USE_MEM_SWAP
#define F_COPY_CACHE_SIZE	32


#ifdef FS_DISK_ROM_FLASH					/**< ¿¿FLASH¿¿¿¿? */
#define SEGMENT_SIZE		512				/**< FLASH¿¿¿¿¿¿¿¿? */
#define MAX_WRITE_UNIT	32					/**< FLASH¿¿¿¿¿¿? */
#endif

#ifdef FS_DISK_SPI_FLASH					/**< ¿¿FLASH¿¿¿¿? */
#define SEGMENT_SIZE		4096				/**< FLASH¿¿¿¿¿¿¿¿? */
#define MAX_WRITE_UNIT	256				/**< FLASH¿¿¿¿¿¿? */
#endif

#ifdef FS_DISK_RAM_FLASH					/**< ¿¿FLASH¿¿¿¿? */
#define SEGMENT_SIZE		64				/**< FLASH¿¿¿¿¿¿¿¿? */
#define MAX_WRITE_UNIT	32					/**< FLASH¿¿¿¿¿¿¿¿? */
#endif

#ifdef FS_ENABLE_BLOCK_MGMT
#define FS_USE_SEGMENT_MAX	1000
#define SEGMENT_ERASE_MAX	500
#else
#define FS_USE_SEGMENT_MAX	0
#endif

#endif
