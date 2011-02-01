/*******************************
修改menu.lst
  title XXXXOS (2.6.XX)
  root (hd0,X)
  kernel /boot/vmlinuz-2.6.XXX ro root=XXX quiet vga=0x318

在kernel那行最后添加vga=0x318或vga=792 （前面十六进制，后面十进制，参考下表）

vga=可设置的值可以用工具fbset（没有的话用sudo apt-get install fbset装）,/etc/fb.modes,或sudo hwinfo --framebuffer参考，主要还是以hwinfo为主（sudo apt-get install hwinfo）

下表列出一些常用值：

# FRAMEBUFFER RESOLUTION SETTINGS
# +----------------------------------------------------------------+
# |    640x480    800x600   1024x768 1280x1024 1280x800   1600x1200
# ----+-------------------------------------------------------------
# 256 | 0x301=769 0x303=771 0x305=773 0x307=775            0x31C=796
# 32K | 0x310=784 0x313=787 0x316=790 0x319=793 0x360＝864 0x31D=797
# 64K | 0x311=785 0x314=788 0x317=791 0x31A=794 0x361＝865 0x31E=798
# 16M | 0x312=786 0x315=789 0x318=792 0x31B=795 0x362＝866 0x31F=799
# +----------------------------------------------------------------+
*******************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "fb_draw.h"

#define D 50
#define R D/2 - 5
#define MAXBOARD 50

enum {NONE = 0, BLACK, WHITE};
enum {LEFT = 1, RIGHT, TOP, DOWN, LEFT_TOP, RIGHT_DOWN, LEFT_DOWN, RIGHT_TOP};
static int BW, boardx, boardy, board[MAXBOARD][MAXBOARD];
static int che_count, winner, space;

static int checkwin(int, int);
extern void printdig(int, int, int);

static void checkpoint(int m, int n, int way) {
	int flag;

	if (BW == 2)
		flag = 1;
	else
		flag = 2;

	switch (way) {
		case LEFT_TOP:
			m--;
			n--;
			break;
		case TOP:
			m--;
			break;
		case RIGHT_TOP:
			m--;
			n++;
			break;
		case LEFT:
			n--;
			break;
		case RIGHT:
			n++;
			break;
		case LEFT_DOWN:
		 	m++;
			n--;
			break;
		case DOWN:
			m++;
			break;
		case RIGHT_DOWN:
			m++;
			n++;
			break;
		default:
			break;
	}

	if (board[m][n] == flag) {
			che_count++;
			checkpoint(m, n, way);
	} else if (board[m][n] == NONE)
			space++;
	return;
}

static void draw_board(int m, int n) {
	int i, j, x, y, len;
	
			x = 100 + n * D ;
			y = 100 + m * D ;
	
			for (i = x - R; i <= x +R; i++)
				for (j = y - R; j <= y + R; j++) {
					switch (board[m][n]) {
					case BLACK:
						len = sqrt((x - i)*(x - i) + (y -j)*(y -j));
						if (len <= R) {
							fb_draw_point(i, j, 0x00);
							fb_draw_point(xres() - 200 + D / 2 + (x - i), R + 10 + (y -j), 0xFF);
						}
						break;
					case WHITE:
						len = sqrt((x - i)*(x - i) + (y -j)*(y -j));
						if (len <= R) {
							fb_draw_point(i, j, 0xFF);
							fb_draw_point(xres() - 200 + D / 2 + (x - i), R + 10 + (y -j), 0x00);
						}
						break;
					case NONE:
						break;
					default:
						break;
					}	
				}
}

static void init_board(void) {
	int i, j, startx = 100, starty = 100, endx=xres() - 100, endy = yres() - 100;
	BW = 1, che_count = 1, winner = 1, space = 0;
	
	boardx = (endx - 100) / D > MAXBOARD ? MAXBOARD: (endx - 100) / D;
	boardy = (endy - 100) / D > MAXBOARD ? MAXBOARD: (endy - 100) / D;
   
	system("clear");
	printf("Enter your setp:\nFormat: x y \n");

	for (i = 0; i < D; i++)
		for (j = 10; j <= D; j++)
			 fb_draw_point(i + xres() - 200, j, 0xfe);
			

	for (i = 1; i <= boardx; i++) {
		printdig(startx - 10 + i * D, starty - 20, i);
		printdig(startx - 10 + i * D, endy + 20, i);
	}

	for (i = 1; i <= boardy; i++) {
		printdig(startx - 40, starty - 10 + i * D, i);
		printdig(endx + 40, starty -10 +i * D, i);
	}

	for (i = 1; i <= boardy; i++)
		for (j = 1; j <= boardx; j++)	
			board[i][j] = 0;

	for (; startx <= endx; startx += D) 
		for (starty = 100; starty <= endy; starty +=D) 
			for (i = 0; i <= D; i++)
				for (j = 0; j <= D; j++)
					if ((i < D - 1) && (j < D -1) )
						fb_draw_point(i+startx ,j+starty ,0xFe);
					else
						fb_draw_point(i+startx ,j+starty ,0x00);
}

static int read_step(void) {

	int c, m = 0, n = 0, flag = 1;

	while ((c = getchar()) != '\n') {
		if (c == EOF)
			return 0;
		if (c == ' ' || c == '\t' || c == ',' || c == '.') {
			flag = 0;
			continue;
		}
		if (c < '0' || c > '9') {
			m = 0;
			break;
		}

		if (flag) 
			m = m * 10 + c - '0';
		else
			n = n * 10 + c - '0';
	}

	if (c != '\n')
		while ((c = getchar()) != '\n') 
			;
	
	if (m > boardy || m < 1 || n > boardx || n < 1) {
		printf("Over\n board!\n");
	} else if (board[m][n]) {
		printf("Can't\n over!\n");
	} else if (BW == 1 ) {
		board[m][n] = 1;
		BW = 2;
	} else { 
		board[m][n] = 2;
		BW = 1;
	}

	draw_board(m, n);
	return ( checkwin(m, n) );
}

static int checkwin(int m, int n){
	int i;
	
	for (i = 1; i <= 8; i += 2) {
		che_count = 1;
		space = 0;
	
		checkpoint(m, n, i);
		if (che_count == 5) {
			printf("Winner!\n");
		return 0;
		}

		checkpoint(m, n, i+1);
		if (che_count == 5 || (che_count == 4 && space == 2) ) {
			printf("Winner!\n");
		return 0;
		}
	}

	return 1;
}


int main(void)
{
   int ret, c;

   ret = fb_open();
   if (ret != 0) {
	fprintf(stderr, "fb_open() error.\n");
		exit(1);
   }
   
	do {
		init_board();
		while (read_step())
			;
		printf("Again?\n(y/n)");
		c = getchar();
		while (getchar() != '\n')
			;
	} while (c == 'y' || c == 'Y');

   fb_close();
   return 0;
}
