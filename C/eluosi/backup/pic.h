#ifndef HS_FB_DRAW_H
#define HS_FB_DRAW_H

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/fb.h>

#define BMPFILE 0x4D42

typedef struct bmpfileheader {
      short int bfType;
      int bfSize;
      short int bfReserved1;
      short int bfReserved2;
      int bfOffBits;
} __attribute__ ((packed)) BMP_FH;

typedef struct tagBITMAPINFOHEADER { /* bmih */
     unsigned int biSize;
     unsigned int biWidth;
     unsigned int biHeight;
     unsigned short int biPlanes;
     unsigned short int biBitCount;
     unsigned int biCompression;
     unsigned int biSizeImage;
     unsigned int biXPelsPerMeter;
     unsigned int biYPelsPerMeter;
     unsigned int biClrUsed;
     unsigned int biClrImportant;
} __attribute__ ((packed)) BMP_FI;

struct color_v {
	char b;
	char g;
	char r;
	char t;
};

int background_pic(char *filepath);
extern void fb_draw_point_v2(int x, int y, struct color_v *color);
extern void fb_draw_point_v3(int x, int y, struct color_v *color);

#endif
