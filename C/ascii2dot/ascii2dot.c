#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define    ASC_12_OFFS  0
#define    ASC_16_OFFS	 (ASC_12_OFFS + 1536)
#define    ASC_24_OFFS	 (ASC_16_OFFS + 4096)
#define    HZK_16_OFFS	 (ASC_24_OFFS + 12288)
#define    HZK_24_OFFS	 (HZK_16_OFFS + 267616)
#define	  HZK_14_OFFS	 (784064) //size = 189504
#define	  ASC_14_OFFS	 (HZK_14_OFFS + 189504)

#define LCD_ROW	16			//必需按8对齐
#define LCD_LINE	16
#define LCD_LINE_EMPTY	0	//字符之间隔一个像素

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

    FONT_ERR,
} FONT_TYPE_T;

static uint8_t LCD[LCD_LINE * (LCD_ROW + 7 )/8];	//屏幕点阵文件
static void lcd_dump(void) {
	int i, j, k;
	//fwrite(LCD, 1, sizeof(LCD), stderr);
	
	for (i = 0; i < LCD_LINE; i++) {
		for (j = 0; j < LCD_ROW / 8; j++)
			for (k = 7; k >= 0; k--)
				printf("%s", LCD[i * LCD_ROW / 8 + j] & (1 << k) ? "--" : "  ");
		printf("\n");
	}
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
	    ret = !is_hz ? ASC_12: FONT_ERR;
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
		default:
	    	break;	
  }

	spi_read(offset, bit_buf, len);

	return len;
}

//字节正序，横向取摸
//从低位到高位，位为0表示描背景色，位为1表示为字体颜色；
static void send_8bit(FONT_TYPE_T font_type, uint8_t tmp) {
   int i;
   static int j = 0;
   int flag = 0;

   for (i = 7; i >= 0; i--)
			printf("%s", tmp & (1 << i) ? "--" : "  ");

   j += 8;
	flag = j >= font_bit_size[font_type].r;

   if (flag) {
		printf("\n");
		j = 0;
   }
}


static int start_x, start_y, end_x, end_y;
static uint8_t byte_rev(uint8_t data) {
	uint8_t val = 0;
	int i;
	for (i = 7; i >= 0; i--) {
		val |= (((data >> i) & 0x1) << (7 - i));
	}
	return ~val;
}

static void send_bitmap(FONT_TYPE_T font_type, uint8_t *tmp) {
   int i, j, row, line, k, n, n1;
	uint8_t c, *lcd;

	line = font_bit_size[font_type].l;
	row = (font_bit_size[font_type].r + 7) / 8;

	for (i = 0; i < line; i++)
		for (j = 0; j < row; j++) {
#if 0
			LCD[(start_y + i) * ((LCD_ROW / 8 -1 )- (start_x/8 + j))] = byte_rev(tmp[i * row + (j)]);
#else
			c = byte_rev(tmp[i * row + j]);
			n = (start_y + i * LCD_ROW) + (start_x + j * 8);	//计算出第n个bit
			for (k = 0; k < 8; k++) {
				n1 = n + k;
				//n1 = n1 / LCD_ROW * LCD_ROW + (LCD_ROW - n1 % LCD_ROW); //倒显
				//printf("set (%d + %d , %d + %d + %d) bit %d = %d, %d[%d], %d\n", \
				//	start_y, i * LCD_ROW,start_x,j*8,k, n+k, (((c>>(7-k))&0x1)<<((n+k)%8)), c, k, n);
				printf("set bit %d to %d, %d,%d,%d\n", n1, (((c>>(7-k))&0x1)<<(n1%8)), c, k, n);
				lcd = &LCD[n1/8];
				*lcd &= ~(1 << (n1%8));
				*lcd |= (((c>>(7-k))&0x1)<<(n1%8));	//置LCD的第n个bit
			}
			printf("lcd[%d]=%d\n", n/8, LCD[n/8]);
#endif
		}
}

static void MainLCD_Window_Set(int cur_row, int cur_line, int next_row, int next_line) {
	//x的起止地址按8bit对齐
	start_x = cur_row;
	start_y = cur_line;
	end_x = next_row;
	end_y = next_line;
}

//设置屏幕起始和结束为止,并将
//row, line指向下一个空白位置,可能换行也可能跳到行首
static void set_lcd_row_line(FONT_TYPE_T font_type, int *rows, int *lines) {
	int font_size_r, font_size_l, cur_row = *rows, cur_line = *lines, next_row, next_line;

	font_size_r = font_bit_size[font_type].r;
	font_size_l = font_bit_size[font_type].l;

	if (cur_row + font_size_r > LCD_ROW) {
		cur_row = 0;
		cur_line += font_size_l + LCD_LINE_EMPTY;
	}

	if (cur_line + font_size_l > LCD_LINE) {
		cur_line = 0;
		cur_row = 0;
	}

	next_row = 	 cur_row + font_size_r;
	next_line =  cur_line + font_size_l;

	MainLCD_Window_Set(cur_row, cur_line, next_row-1, next_line-1);

	next_row += LCD_LINE_EMPTY;
	next_line += LCD_LINE_EMPTY;

 	*rows = next_row;
	*lines = cur_line;

}

static void lcd_print(FONT_SIZE_T size, int row, int lines, const uint8_t *str) {
	unsigned char is_hz;
	unsigned char bit_buf[FONT_MAX * (FONT_MAX/8)];
	int i, j;
	FONT_TYPE_T font_type;

	while (*str != '\0') {
		is_hz = (*str) > 0xa0 ? 1 : 0;	//判断是否为汉字	
		//返回字体类型
		font_type = get_word_type(size, is_hz);
		//设置屏幕输出的起始位置
		set_lcd_row_line(font_type, &row, &lines);

		//从字库中取出当前字的点阵, 并返回总共的字节数
		memset(bit_buf, 0x0, sizeof(bit_buf));
		j = get_bitmap(font_type, bit_buf, str);
		send_bitmap(font_type, bit_buf);
		for (i = 0; i < j; i++) { 
		    //从低位到高位，位为0表示描背景色，位为1表示为字体颜色；
		    //send_8bit(font_type, bit_buf[i]);
		}
		//row, line始终指向下一个空白位置,可能换行也可能跳到行首
		str = is_hz ? str + 2 : str + 1;	//指向下一个字符
	}
}

int main(int argc, char **argv) {
	memset(LCD, 0xff, sizeof(LCD));
	//lcd_print(FONT_14, 0, 0, (uint8_t *)"abc一二三四@!好的子额个会自动换行");
	lcd_print(FONT_16, 0, 0, (uint8_t *)"还有");
	lcd_dump();
	return 0;
}
