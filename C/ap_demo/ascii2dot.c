#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "datatype.h"

//字节正序，横向取摸
//0x82[10000010] 对于像素为*_____*_

#define    ASC_12_OFFS  0
#define    ASC_16_OFFS	 (ASC_12_OFFS + 1536)
#define    ASC_24_OFFS	 (ASC_16_OFFS + 4096)
#define    HZK_16_OFFS	 (ASC_24_OFFS + 12288)
#define    HZK_24_OFFS	 (HZK_16_OFFS + 267616)
#define	  HZK_14_OFFS	 (784064) //size = 189504
#define	  ASC_14_OFFS	 (HZK_14_OFFS + 189504)
#define	  HZK_12_OFFS	 974898 //新宋 9号字体，12x12，size = 162432

typedef enum {
    FONT_12	= 12,
    FONT_16	= 16,
    FONT_24	= 24,
	 FONT_14 = 14,

    FONT_MAX	= FONT_24,
} FONT_SIZE_T;

typedef enum {
    ASC_12,
    ASC_16,
    ASC_24,
	 ASC_14,
    HZK_16,
    HZK_24,
	 HZK_14,
	 HZK_12,

    FONT_ERR,
} FONT_TYPE_T;

typedef struct LCD_T {
	int	row;
	int	line;
	int 	real_row;

	int lcd_line_empty;

	int start_x;
	int start_y;

	uint8_t *buf;

} LCD_T;

static LCD_T *lcd_init(int row, int line) {
	LCD_T *lcd;

	lcd = malloc(sizeof(LCD_T));
	lcd->buf = malloc(line * ((row + 7 )/8));

	if (lcd == NULL || lcd->buf == NULL) {
		free(lcd->buf); free(lcd);
		return NULL;
	}

	lcd->lcd_line_empty = 0;
	lcd->real_row = row;
	lcd->row = (row + 7) / 8 * 8;
	lcd->line = line;

	memset(lcd->buf, 0xff, line * ((row + 7 )/8));

	return lcd;
}

//按照纵向的方式存储文件
static void lcd2dot(LCD_T *lcd, uint8_t *buf) {
   uint8_t c, line_buf[lcd->line];   //一次纵向取line个字符
   int k, row, line, B, n = 0;

   for (row = 0; row < (lcd->row +7) /8; row++) {
      //取一纵向的字节到line中
      for (line = 0; line < lcd->line; line++)
         line_buf[line] = lcd->buf[line * ((lcd->row + 7) / 8) + row];
      //将它们的各位纵向合并成一个字节
      for (k = 7; k >= 0; k--) {
         for (B = 0; B < lcd->line; B += 8) { //纵向8个一组
            c = 0;
            c |= (((line_buf[B + 0] >> k) & 1) << 7); 
            c |= (((line_buf[B + 1] >> k) & 1) << 6); 
            c |= (((line_buf[B + 2] >> k) & 1) << 5); 
            c |= (((line_buf[B + 3] >> k) & 1) << 4); 
            c |= (((line_buf[B + 4] >> k) & 1) << 3); 
            c |= (((line_buf[B + 5] >> k) & 1) << 2); 
            c |= (((line_buf[B + 6] >> k) & 1) << 1); 
            c |= (((line_buf[B + 7] >> k) & 1) << 0); 
            buf[n++] = c;
         }   
         //最后几个bit可能凑不齐一个字节，丢弃之
         //NEW_LCD[n-1] &= (0xff << (7 - LCD_LINE % 8));
			if (lcd->line % 8)
				n--;
      }   
   }   
}

static int lcd_flush(LCD_T *lcd, uint8_t *buf) {
	uint16_t xy[] = {0, 0, lcd->line / 8 -1, lcd->real_row -1};

	memcpy(buf, xy, sizeof(xy));
	lcd2dot(lcd, buf + sizeof(xy));	//头8个字节为xy坐标

#if 0
	int i, j, k;
	for (i = 0; i < lcd->line; i++) {
		for (j = 0; j < lcd->row / 8; j++)
			for (k = 7; k >= 0; k--)
				//printf("%s", lcd->buf[i * lcd->row / 8 + j] & (1 << k) ? "--" : "  ");
				fb_draw_point(i, j*8 + k, lcd->buf[i * lcd->line / 8 + j] & (1 << k) ? 0x0000 : 0xffff);
		printf("\n");
	}
//#else
	int i, j, k;
	for (i = 0; i < lcd->row; i++) {
		for (j = 0; j < lcd->line / 8; j++)
			for (k = 7; k >= 0; k--) {
				//printf("%s", buf[i * LCD_LINE / 8 + j] & (1 << k) ? "--" : "  ");
				fb_draw_point(i, j*8 + 7 - k, buf[i * lcd->line / 8 + j + 8] & (1 << k) ? 0x0000 : 0xffff);
				}
	}
#endif
	free(lcd->buf); free(lcd);
	return lcd->real_row * lcd->line /8 + 8;
}

static void spi_read(uint32_t addr, uint8_t *buf, int len) {
	FILE *fp;

	if ((fp = fopen("font.bin", "r")) == NULL) {
		perror("fopen:");
		return;
	}
	fseek(fp, addr, SEEK_SET);
	fread(buf, 1, len, fp);
	fclose(fp);
}

//返回字体类型
FONT_TYPE_T get_word_type(FONT_SIZE_T size, uint8_t is_hz) {
  FONT_TYPE_T ret;

  switch (size){
	case FONT_12:
	    ret = !is_hz ? ASC_12: HZK_12;
	    break;
	case FONT_16:
	    ret = !is_hz ? ASC_16: HZK_16;
	    break;
	case FONT_24:
	    ret = !is_hz ? ASC_24: HZK_24;
	    break;
	case FONT_14:
	    ret = !is_hz ? ASC_14: HZK_14;
	    break;
	default:
	    ret = FONT_ERR;
	    break;
  }

  return ret;
}

struct __font_bit_size {
	uint8_t r;		//宽度
	uint8_t l;		//高度
	uint8_t s;		//占用字节数
};
static const struct __font_bit_size font_bit_size[] = {
	{8,12,12},		//ASC_12
	{8,16,16},		//ASC_16
	{16,24,48},		//ASC_24
	{8,14,14},		//ASC_14
	{16,16,32},		//HZK_16
	{24,24,72},		//HZK_24
	{14,14,28},		//HZK_14
	{12,12,24},		//HZK_12	
	{0,0,0},		//error
};

static uint8_t get_bitmap(FONT_TYPE_T font_type, uint8_t *bit_buf, const uint8_t *str) {
	uint32_t offset;
	int len = font_bit_size[font_type].s;

	switch (font_type) {
		case ASC_12:
			offset = ASC_12_OFFS + (*str) * len;		
	    	break;
		case ASC_16:
	    	offset = ASC_16_OFFS + (*str) * len;		
	    	break;
		case ASC_24:
	    	offset = ASC_24_OFFS + (*str) * len;		
	    	break;
		case ASC_14:
	    	offset = ASC_14_OFFS + (*str - ' ') * len;		
	    	break;
		case HZK_16:
	    	offset = HZK_16_OFFS + (94*(str[0] - 0xa0 -  1) + (str[1] - 0xa0 -1)) * len;		
	    	break;
		case HZK_24:
	    	offset = HZK_24_OFFS + (94*(str[0] - 0xa0  - 15 - 1) + (str[1] - 0xa0 -1)) * len;		
	    	break;
		case HZK_14:
	    	offset = HZK_14_OFFS + (94*(str[0] - 0xa0  - 15 - 1) + (str[1] - 0xa0 -1)) * len;		
	    	break;
		case HZK_12:
	    	offset = HZK_12_OFFS + (94*(str[0] - 0xa0  - 15 - 1) + (str[1] - 0xa0 -1)) * len;		
	    	break;
		default:
	    	break;	
  }

	spi_read(offset, bit_buf, len);

	return len;
}

static uint8_t byte_rev(uint8_t data) {
	uint8_t val = 0;
	int i;
	for (i = 7; i >= 0; i--) {
		val |= (((data >> i) & 0x1) << (7 - i));
	}
	return ~val;
}

static void set_arr_bit(uint8_t *arr, int bitn, int val) {
	uint8_t *p = &arr[bitn / 8];
	//printf("set arr bit %d to %d\n", bitn, val);
	*p &= ~(1 << (7 - bitn % 8));
	*p |= ((val & 1) << (7 - bitn % 8));	//置LCD的第n个bit



	//fb_draw_point(bitn % 72, bitn / 72, val ? 0x0000 : 0xffff);
}

static void send_bitmap(FONT_TYPE_T font_type, uint8_t *tmp, LCD_T *lcd) {
	uint8_t c;
	int bit, line, row, k;
	int font_row = font_bit_size[font_type].r, font_low_align = (font_row + 7) / 8;

	for (line = 0; line < font_bit_size[font_type].l; line++) {
		//每次从tmp中取出一个字的一行
		for (row = 0, bit = 0; row < font_low_align; row++) {
				c = byte_rev(tmp[line * font_low_align + row]);
				for (k = 0; k < 8; k++) { //依次置位
					//但要跳过最后若干位，比如14像素宽的字体，第2个字节的高2位是不需要的
					if (row == (font_low_align -1) && font_row % 8 != 0 && k >= font_row % 8) {
						//printf("skip %d, k = %d\n", row, k);
						continue;
					}
					set_arr_bit(lcd->buf, (lcd->start_y + line) * lcd->row + lcd->start_x + bit, (c >> k) & 1);
					bit++;
				}
		}
	}
}

//设置屏幕起始和结束为止,并将
//row, line指向下一个空白位置,可能换行也可能跳到行首
static void set_lcd_row_line(FONT_TYPE_T font_type, int *rows, int *lines, LCD_T *lcd) {
	int font_size_r, font_size_l, cur_row = *rows, cur_line = *lines, next_row, next_line;

	font_size_r = font_bit_size[font_type].r;
	font_size_l = font_bit_size[font_type].l;

	if (cur_row + font_size_r > lcd->row) {
		cur_row = 0;
		cur_line += font_size_l + lcd->lcd_line_empty;
	}

	if (cur_line + font_size_l > lcd->line) {
		cur_line = 0;
		cur_row = 0;
	}

	next_row = 	 cur_row + font_size_r;
	next_line =  cur_line + font_size_l;

	lcd->start_x = cur_row;
	lcd->start_y = cur_line;

	next_row += lcd->lcd_line_empty;
	next_line += lcd->lcd_line_empty;

 	*rows = next_row;
	*lines = cur_line;

}

static void lcd_print(FONT_SIZE_T size, int row, int lines, const uint8_t *str, LCD_T *lcd) {
	unsigned char is_hz;
	unsigned char bit_buf[FONT_MAX * (FONT_MAX/8)];
	FONT_TYPE_T font_type;

	while (*str != '\0') {
		is_hz = (*str) > 0xa0 ? 1 : 0;	//判断是否为汉字	
		//返回字体类型
		font_type = get_word_type(size, is_hz);
		//设置屏幕输出的起始位置
		set_lcd_row_line(font_type, &row, &lines, lcd);

		//从字库中取出当前字的点阵
		memset(bit_buf, 0x0, sizeof(bit_buf));
		get_bitmap(font_type, bit_buf, str);
		send_bitmap(font_type, bit_buf, lcd);
		//row, line始终指向下一个空白位置,可能换行也可能跳到行首
		str = is_hz ? str + 2 : str + 1;	//指向下一个字符
	}
}

extern int protocal_data(const uint8_t *content, int content_len, uint8_t *buf, int buf_len);

int lcd_display(const struct dot_info_t *info, uint8_t *out_buf, int out_len) {
	int len = 0, line, row;
	LCD_T *lcd;
	uint8_t *lcd_dot;
	char price[64];

	switch (info->type) {
		case 29:
			line = 296;
			row = 128;
			break;
		default:
			line = 172;
			row = 72;
			break;
	}

	if ((lcd_dot = malloc((line+7)/8*8 * (row+7)/8*8)) == NULL) {
		goto _ret;
	}

	if ((lcd = lcd_init(line, row)) == NULL) {
		goto _free_buf;
	}

	snprintf(price, sizeof(price), "%0.2f", info->price);
	
	lcd_print(FONT_12, 0, 0, (uint8_t *)info->name, lcd);
	lcd_print(FONT_12, 0, 48, (uint8_t *)info->origin, lcd);
	lcd_print(FONT_24, 24 * 8, 60, (uint8_t *)price, lcd);
	len = lcd_flush(lcd, lcd_dot);

	//protocol_data失败时返回0，此返回值也会被上层调用者接收到
	len = protocal_data(lcd_dot, len, out_buf, out_len);

_free_buf:
	free(lcd_dot);
_ret:
	return len;
}
