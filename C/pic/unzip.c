#include "datatype.h"
#include "mini_fs.h"
#include <string.h>
#include <stddef.h>
#include "public_temp.h"
#include "unzip.h"
#include "system.h"

file_id_t get_curr_f(void) {return EPD_CFG.lcd_f.curr;}
file_id_t get_buff_f(void) {return EPD_CFG.lcd_f.buf;}
file_id_t get_diff_f(void) {return EPD_CFG.lcd_f.diff;}

static UINT8 *BUF = (void *)&TEMP.BYTE64[0];

static void swap_curr_megr(void) {
	file_id_t tmp;
	tmp = EPD_CFG.lcd_f.curr;
	EPD_CFG.lcd_f.curr = EPD_CFG.lcd_f.megr;
	EPD_CFG.lcd_f.megr = tmp;
}

//********************************************
//**********文件合并函数**********************
//********************************************

//因为这组变量太大，放到全局临时的空间内
struct merger_temp_t {
	WORD y, i, y_off, l1, l2,l3;
	WORD len;
	WORD min_x, min_y, max_x, max_y;
	BYTE buf[PIC_ROW_BYTE];
};

//fp不支持往回写，只能追加写
static UINT8 merger(file_id_t fp, file_id_t prov,  file_id_t diff) {
	struct merger_temp_t *t = (void *)&TEMP.BYTE64[0];	
	//读出更新坐标区域
	f_read(diff, 0, (void *)&(t->min_x), sizeof(t->min_x) * 4);	

	if (t->min_x > t->max_x || t->min_y > t->max_y || \
		t->max_x >= PIC_ROW_BYTE || t->max_y >= PIC_ROW_COUNT) 
	{
		gEventFlag |= EVENT_FLAG_ZIP_ERR;
		return 1;	
	}

	t->len = f_len(fp);
	
	//先写头min_y行
	for (t->y = 0, t->y_off = 0; t->y < t->min_y; t->y++, t->y_off += PIC_ROW_BYTE)
		t->len += f_copy(fp, t->len, prov, t->y_off, PIC_ROW_BYTE);

	//中间部分共max_y - min_y+1行，每行从prov_buf取前l1个字节，
	//从diff_buf取l2个字节,再从prov_buf中取l3个字节
	t->l1 = t->min_x;
	t->l2 = t->max_x - t->min_x + 1;
	t->l3 = PIC_ROW_BYTE - t->max_x - 1;
	for (t->y = t->min_y, t->i = 0; t->y <= t->max_y; t->y++, t->y_off += PIC_ROW_BYTE, t->i += t->l2) {
		f_read(prov, t->y_off, t->buf, PIC_ROW_BYTE);
		f_read(diff, t->i + sizeof(int)*4, &t->buf[t->l1], t->l2);	
		f_write(fp, t->len, t->buf, PIC_ROW_BYTE);
		t->len += PIC_ROW_BYTE;	
	}
	
	//最后写max_y - PIC_ROW_COUNT行
	for (t->y = t->max_y + 1; t->y < PIC_ROW_COUNT; t->y++, t->y_off += PIC_ROW_BYTE)
		t->len += f_copy(fp, t->len, prov, t->y_off, PIC_ROW_BYTE);
	
	if (t->len > f_size(fp)) {
		gEventFlag |= EVENT_FLAG_FS_OVER;
		return 1;
	}

	return 0;
}

//********************************************
//**********解压缩函数************************
//********************************************

//哈夫曼解码
static UINT8 haffman_decode(file_id_t in, WORD offset, WORD len, file_id_t out) {
	int i, j;
	UINT8 *mem_arr = TEMP.HAFF_CODE;
	unsigned char data, p;
	UINT32 wc = f_len(out);

	//拷贝码元，如果码元超过64个则空间不足，不能解压
	f_read(in, offset, &data, 1);
	if (data > sizeof(TEMP.HAFF_CODE)) {
		gEventFlag |= EVENT_FLAG_ZIP_ERR;
		return 1;
	}
	f_read(in, offset + 1, mem_arr, data);	

	//如何正确的发送完3096个字节呢
	//因为源是3096字节，压缩的时候，最后一个字节的最后若干个位不能组成一个有效码元，
	//也就不会把第3097个字节发送出去
	gEventFlag &= ~EVENT_FLAG_FS_OVER;
	for (i = offset + 1 + data; i < len; i++) {
		f_read(in, i, &data, 1);
		//tmp = data[i];
		for (j = 7; j >= 0; j--) {
			if (data & (1 << j)) {
				// 找到一个1, 表面这是一个需要解压的字节结束位
				if (f_write(out, wc++, &mem_arr[p], 1) != 1) {
					gEventFlag |= EVENT_FLAG_FS_OVER;
					return 1;				
				}
				p = 0;
			} else {
				p++;	// 找到一个0,不是结束符号
			}
		}
	}

	return 0;
}

//REL8解码
#define KEY1	0x00	//	0x00需要解压缩
#define KEY2	0xff	//	0xff需要解压缩
static UINT8 rel8_decode(file_id_t in, WORD offset, WORD len, file_id_t out) {
	UINT8 c, n, n1;
	WORD i, j;
	UINT32 wc = f_len(out);
	UINT8 *buf = (void *)&TEMP.BYTE64[32]; //头32个字节给read用，后32字节给write用

   //读取的字节数应该为len-起始地址
	len -= offset;
	do {
		//还剩多少个字节
		if (len  > 32)
			n = 32;
		else
			n = len;
		//读取n个字节
		f_read(in, offset, BUF, n);
		len -= n;
		offset += n;
		//解压n个字节
		for (i = 0; i < n; i++) {
			c = BUF[i];
			switch (c) {
				case KEY1:	//碰到00或者ff则连续输出n个，n由下一个字节决定
				case KEY2:
					i++;
					if (i == 32) {
						//重复的次数没有读出来, 则读1个字节
						f_read(in, offset, &n1, 1);
						len--;
						offset++;
					} else
						n1 = BUF[i];
					//一次写32个同样的字符
					memset(buf, c, 32);
					for (j = n1 / 32; j > 0; j--, wc += 32, n1 -= 32)
						f_write(out, wc, buf, 32);
					if (n1 > 0) {
						f_write(out, wc, buf, n1);
						wc += n1;
					}	
					break;
				default:
					f_write(out, wc++, &c, 1);
					break;
			}			
		}

	
	} while (len != 0);

   return 0;
}

//********************************************
//**********文件解析函数**********************
//********************************************

enum {NOZIP, REL8, HAFFMAN, UNKNOWN,};

struct lcd_file_flag_t {
	UINT8 unused:5;
	UINT8 block_flag:1;
	UINT8 zip_flag:2;
};
#pragma pack(1)
typedef struct FILE_HEAD_T {
	UINT16 crc;
	UINT32 size;
	UINT8 flag;
} FILE_HEAD_T;
#pragma pack()

static FILE_HEAD_T h;
static volatile struct lcd_file_flag_t *flag = (struct lcd_file_flag_t *)&h.flag;

#define SIZEOF(s,m) ((size_t) sizeof(((s *)0)->m))
static UINT8 decode_file(file_id_t in, file_id_t out) {
	UINT32 size = h.size;
	typedef UINT8 (*decode_fun_t)(file_id_t in, WORD offset, WORD len, file_id_t out);
	decode_fun_t decode_fun[] = {NULL, rel8_decode, haffman_decode};
	UINT8 ret = 0;
	
	if (flag->zip_flag >= UNKNOWN) {
		gEventFlag |= EVENT_FLAG_ZIP_ERR;
		ret = 1;
	} else if (flag->zip_flag == NOZIP) {
		if (f_copy(out, 0, in, sizeof(FILE_HEAD_T), size - sizeof(FILE_HEAD_T))!= size - sizeof(FILE_HEAD_T)) {
			gEventFlag |= EVENT_FLAG_FS_OVER;
			ret = 1;
		}	
	} else {
		ret = (decode_fun[flag->zip_flag])(in, sizeof(FILE_HEAD_T), size, out);
	}	
	
	return ret;
}


//CRC16, 多项式1021
static UINT8 file_check(file_id_t fd) {
	UINT16 crc = 0;
	UINT32 i, j, n, off, size;

	gEventFlag &= ~(EVENT_FLAG_BUF_CRC + EVENT_FLAG_ZIP_ERR + EVENT_FLAG_FS_OVER);
	
	f_read(fd, 0, (UINT8 *)&h, sizeof(FILE_HEAD_T));	
	size = h.size;

	//  5 < size < 临时文件的空间
	if (size > f_size(fd) || size < sizeof(FILE_HEAD_T) - offsetof(FILE_HEAD_T, size)) {
		gEventFlag |= EVENT_FLAG_FS_OVER;
		return 1;
	}
	//block方式暂不处理，只支持xy坐标方式更新
	if (flag->block_flag == 1) {				
		gEventFlag |= EVENT_FLAG_ZIP_ERR;
		return 1;
	}	
	

	j = offsetof(FILE_HEAD_T, size);

//采用buff的方式读取待验证的字节
	off = j;
	do {
		//还剩多少个字节
		if (size > 32)
			n = 32;
		else
			n = size;
		//读取n个字节
		f_read(fd, off, BUF, n);
		size -= n;
		off += n;
		//计算n个字节的CRC
		for (j = 0; j < n; j++) {
			for(i = 0x80; i != 0; i /= 2) {
				crc *= 2;
				if((crc & 0x8000) != 0)
					crc ^= 0x1021;
				if((BUF[j] & i) != 0) 
					crc ^= 0x1021;
			}		
		}
	
	} while (size != 0);
		
	//检查结果
	if (h.crc != crc) {
		gEventFlag |= EVENT_FLAG_BUF_CRC;
		return 1;
	}

	return 0;
}

void unzip_file_end(void) {
	f_erase(EPD_CFG.lcd_f.diff);
	f_erase(EPD_CFG.lcd_f.buf);
	f_erase(EPD_CFG.lcd_f.megr);
	f_write(F_EPD_CFG, 0, (void *)&EPD_CFG, sizeof(EPD_CFG));
	f_sync();
}

//数据完全收完后，调用此函数解压缩数据
//16M频率处理单色全帧图像 crc 100ms，decode 14ms，合并32ms，擦除buf 200ms
//改用32字节的fifo后，解码847字节的zel8数据，crc 13ms，decode 58ms，megra 42ms
//如果解码不压缩的1563字节，crc 24ms，decode 15ms，megra 42ms
//擦除diff，再擦除buff，保存文件，保存文件系统各耗时 548ms，184ms，296ms，228ms, 共1.256s
UINT8 unzip_file(void){
	UINT8 ret = 1;

   if ((ret = file_check(EPD_CFG.lcd_f.buf)) != 0)
		goto _ret;
	//解压数据到diff
	if ((ret = decode_file(EPD_CFG.lcd_f.buf, EPD_CFG.lcd_f.diff)) != 0)	
		goto _ret;
	if (merger(EPD_CFG.lcd_f.megr, EPD_CFG.lcd_f.curr, EPD_CFG.lcd_f.diff) != 0)	//合并curr和diff，保存到buf
		goto _ret;
	ret = 0;	
	swap_curr_megr();	//交换curr和buf的值，使的curr永远指向归并后的文件。
_ret:
	gEventFlag |=  EVENT_FLAG_CL_UNZIP;

	return ret;	
}

#include "epd.h"
extern void ap_demo_test(void);
UINT8 unzip_test(void) {
	//ap_demo_test();
	//extern const unsigned char gs_image[1548 + 13 + 2];
	extern const unsigned char tiaoma_rel8[2742 + 5 +2];
	f_write(get_buff_f(), 0, tiaoma_rel8, sizeof(tiaoma_rel8));
	unzip_file();
	unzip_file_end();
	//Init_EPD();
	
	return 0;
}





