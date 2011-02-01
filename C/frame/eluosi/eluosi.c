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

#define D 30
#define R D/2 - 5
#define MAXBOARD 50

enum {NONE = 0, BLACK, WHITE};
static int BW, boardx, boardy, board[MAXBOARD][MAXBOARD];
static int che_count, winner, space;

//static int checkwin(int, int);
extern void printdig(int, int, int);


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
	int i, j, startx = 50, starty = 50, endx=xres() - 50, endy = yres() - 50;
	int BW = 1, che_count = 1, winner = 1, space = 0, col = 0;
	
	boardx = (endx - 100) / D > MAXBOARD ? MAXBOARD: (endx - 100) / D;
	boardy = (endy - 100) / D > MAXBOARD ? MAXBOARD: (endy - 100) / D;
   
	system("clear");


	for (; startx <= endx - 400 ; startx += D) 
		for (starty = 100; starty <= endy; starty +=D) {
			for (i = 0; i <= D; i++)
				for (j = 0; j <= D; j++)
					if ((i < D - 1) && (j < D -1) ) {
						fb_draw_point(i+startx ,j+starty , 244);
					} else
						fb_draw_point(i+startx ,j+starty ,0x00);
				//printdig(i+startx ,j+starty , col++);
			}
}



int main(void)
{
   int ret, c;

   ret = fb_open();
   if (ret != 0) {
	fprintf(stderr, "fb_open() error.\n");
		exit(1);
   }
   
	init_board();

   fb_close();
   return 0;
}
