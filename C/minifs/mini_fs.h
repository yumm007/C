#ifndef __MINI_FS__
#define __MINI_FS__

typedef unsigned char UINT8;
typedef unsigned int UINT16;

typedef enum {
	FILE1,
	FILE2,
	FILE3,
	FILE4,
	FILE5,
	FILE6,
	FILE7,
	FILE8,

	FILE_ID_END = FILE8,
} file_id_t;

struct file_info_t {
	UINT16 		start_addr;
	UINT16		file_len;
	UINT16		file_size;
};

typedef struct fs_t {
	UINT8 valid;
	UINT8 flag;
	struct file_info_t file[FILE_ID_END + 1];
} fs_t;
extern fs_t fs;

#define SEGMENT_SIZE	32
#define DISK_BLOCK	1000

void 		f_init(void);
void 		f_sync(void);
UINT8*	f_read(file_id_t id, 	UINT16 offset,	UINT16 len);
UINT16 	f_write(file_id_t id, 	UINT16 offset,	const UINT8 *data, UINT16 len);
void		f_erase(file_id_t id);

//以下三个函数需要移植，传入的地址为相对于DISK的偏移地址
extern void segment_erase(UINT16 addr);
extern void segment_copy_mem(UINT16 addr, UINT16 offset,  const UINT8 *data, UINT16 len);
extern void segment_copy_segment(UINT16 des, UINT16 src, UINT16 len);

#endif

