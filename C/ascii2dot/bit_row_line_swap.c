#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define LCD_ROW	72
#define LCD_LINE	172

static uint8_t LCD[((LCD_ROW + 7) / 8) * LCD_LINE];
static uint8_t NEW_LCD[LCD_ROW * ((LCD_LINE + 7) / 8)];

//按照纵向的方式存储文件
static void lcd2dot(void) {
	uint8_t c, line_buf[LCD_LINE];	//一次纵向取line个字符
	int k, row, line, B, n = 0;

	for (row = 0; row < (LCD_ROW +7) /8; row++) {
		//取一纵向的字节到line中
		for (line = 0; line < LCD_LINE; line++)
			line_buf[line] = LCD[line * ((LCD_ROW + 7) / 8) + row];
		//将它们的各位纵向合并成一个字节
		for (k = 7; k >= 0; k--) {
			for (B = 0; B < LCD_LINE; B += 8) { //纵向8个一组
				c = 0;
				c |= (((line_buf[B + 0] >> k) & 1) << 7);
				c |= (((line_buf[B + 1] >> k) & 1) << 6);
				c |= (((line_buf[B + 2] >> k) & 1) << 5);
				c |= (((line_buf[B + 3] >> k) & 1) << 4);
				c |= (((line_buf[B + 4] >> k) & 1) << 3);
				c |= (((line_buf[B + 5] >> k) & 1) << 2);
				c |= (((line_buf[B + 6] >> k) & 1) << 1);
				c |= (((line_buf[B + 7] >> k) & 1) << 0);
				NEW_LCD[n++] = c;
			}
			//最后几个bit可能凑不齐一个字节，需要清为0
			//NEW_LCD[n-1] &= (0xff << (7 - LCD_LINE % 8));
			if (LCD_LINE % 8)
				n--;
		}
	}
}

static void arr_dump(void) {
	int i, j;

	for (j = 0; j < LCD_ROW; j++) {
		for (i = 0; i < (LCD_LINE + 0) / 8; i++)
			printf("%02X, ", NEW_LCD[j * ((LCD_LINE+0) /8) + i]);
		printf("\n");
	}
	
}

int main(void) {
	memset(LCD, 0xf, sizeof(LCD));
	lcd2dot();
	arr_dump();
	return 0;
}
