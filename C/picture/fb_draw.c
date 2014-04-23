#include "main.h"

struct fb_st {
     struct fb_fix_screeninfo fix;
     struct fb_var_screeninfo var;
     unsigned long bpp;
     int fd;
     char *fbp;
};

static struct fb_st fb0;
static int otsu(unsigned char *colors, int w, int h);

int fb_open(void)
{
     int ret;

     fb0.fd = open("/dev/fb0", O_RDWR);
     if (-1 == fb0.fd) {
	  perror("open");
	  goto error;
     }

     /* get fb_var_screeninfo */
     ret = ioctl(fb0.fd, FBIOGET_VSCREENINFO, &fb0.var);
     if (-1 == ret) {
	  perror("ioctl(fb0.var)");
	  goto close_fd;
     }

     fb0.bpp = fb0.var.bits_per_pixel / 8;

     /* get fb_fix_screeninfo */
     ioctl(fb0.fd, FBIOGET_FSCREENINFO, &fb0.fix);
     if (-1 == ret) {
	  perror("ioctl(fb0.fix)");
	  goto close_fd;
     }

     /* get framebuffer start address */
     fb0.fbp = mmap(NULL, fb0.fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fb0.fd, 0);
     if ((void *)-1 == fb0.fbp) {
	  perror("mmap");
	  goto close_fd;
     }

     return 0;

close_fd:
     close(fb0.fd);
error:
     return -1;
}

void fb_close()
{
     munmap(fb0.fbp, fb0.fix.smem_len);
     close(fb0.fd);
}

void fb_draw_point(int x, int y, struct color_v *color)
{
     unsigned long offet = 0;
	  char *p;

     offet = fb0.bpp * x + y * fb0.fix.line_length;
	  p = fb0.fbp + offet;

	  memcpy(p, color, fb0.bpp);
}


int init_bmp(BMP_FH *bmp_fh, BMP_FI *bmp_fi, FILE *fp) {

	if (fread(bmp_fh, sizeof(*bmp_fh), 1, fp) < 1 || \
		fread(bmp_fi, sizeof(*bmp_fi), 1, fp) < 1 ) 
		return -1;

	if (bmp_fh->bfType != BMPFILE)	
		return -1;

	fseek(fp, bmp_fh->bfOffBits, SEEK_SET);

	return 0;
}

static int read_color(struct color_v *color, int bpp, FILE *fp) {
	short int a;

	switch (bpp) {
	case 4:
		fread(&color->b, 1, 1, fp);
		fread(&color->g, 1, 1, fp);
		fread(&color->r, 1, 1, fp);
		fread(&color->t, 1, 1, fp);
		break;
	case 3:
		fread(&color->b, 1, 1, fp);
		fread(&color->g, 1, 1, fp);
		fread(&color->r, 1, 1, fp);
		break;
	case 2:
		fread(&a, 2, 1, fp);
#if 1
		color->r = (a & 0xFB00) / 800 * 8;
		color->g = (a & 0x07E0) / 20 * 4;
		color->b = (a & 0x001F)* 8;
#else 
		color->r = ((~((~0) << 5)) & (a >> 11)) / 800 * 8 ;
		color->g = ((~((~0) << 6)) & (a >> 5)) / 20 * 4;
		color->b = ((~((~0) << 5)) & a) * 8;
#endif
		break;
	case 1:
		break;
	default:
		break;
	}
	return 0.299 * color->r + 0.587 * color->g + 0.114 * color->b;
}

unsigned char bmp[4096*1024*4];
void draw_pic(BMP_FI *bmp_fi, FILE *fp, int x, int y) {
	int i, j, bpp, offset, thresh;
	struct color_v color = {0,0,0,0};
	struct color_v white = {0xff, 0xff, 0xff, 0xff};
	struct color_v black = {0, 0, 0, 0};

	bpp = bmp_fi->biBitCount / 8;
	offset = (4 - (bmp_fi->biWidth * bpp) % 4) % 4;

	for (i = bmp_fi->biHeight; i > 0; i--) {
		for (j = 0; j < bmp_fi->biWidth; j++)
			bmp[i * bmp_fi->biWidth + j] = read_color(&color, bpp, fp);
		for (j = offset; j > 0; j--)
			fgetc(fp);
	}
	thresh = otsu(bmp, bmp_fi->biWidth, bmp_fi->biHeight);
	for (i = bmp_fi->biHeight; i > 0; i--) 
		for (j = 0; j < bmp_fi->biWidth; j++) 
			fb_draw_point(x+j, y+i, bmp[i * bmp_fi->biWidth + j] > thresh ? &black : &white);
}


/** 
 * OTSU算法求最适分割阈值 
 */ 
static int otsu(unsigned char *colors, int w, int h) { 
    unsigned int pixelNum[256]; // 图象灰度直方图[0, 255] 
    int color; // 灰度值 
    int n, n0, n1; //  图像总点数，前景点数， 后景点数（n0 + n1 = n） 
    int w0, w1; // 前景所占比例， 后景所占比例（w0 = n0 / n, w0 + w1 = 1） 
    double u, u0, u1; // 总平均灰度，前景平均灰度，后景平均灰度（u = w0 * u0 + w1 * u1） 
    double g, gMax; // 图像类间方差，最大类间方差（g = w0*(u0-u)^2+w1*(u1-u)^2 = w0*w1*(u0-u1)^2） 
    double sum_u, sum_u0, sum_u1; // 图像灰度总和，前景灰度总和， 后景平均总和（sum_u = n * u） 
    int thresh; // 阈值 
 
    memset(pixelNum, 0, 256 * sizeof(unsigned int)); // 数组置0 
 
    // 统计各灰度数目 
    int i, j; 
    for (i = 0; i < h; i++) { 
        for (j = 0; j < w; j++) { 
            color = (colors[w * i + j]) & 0xFF; // 获得灰度值 
            pixelNum[color]++; // 相应灰度数目加1 
        } 
    } 
 
    // 图像总点数 
    n = w * h; 
 
    // 计算总灰度 
    int k; 
    for (k = 0; k <= 255; k++) { 
        sum_u += k * pixelNum[k]; 
    } 
 
    // 遍历判断最大类间方差，得到最佳阈值 
    for (k = 0; k <= 255; k++) { 
        n0 += pixelNum[k]; // 图像前景点数 
        if (0 == n0) { // 未获取前景，直接继续增加前景点数 
            continue; 
        } 
        if (n == n0) { // 前景点数包括了全部时，不可能再增加，退出循环 
            break; 
        } 
        n1 = n - n0; // 图像后景点数 
 
        sum_u0 += k * pixelNum[k]; // 前景灰度总和 
        u0 = sum_u0 / n0; // 前景平均灰度 
        u1 = (sum_u - sum_u0) / n1; // 后景平均灰度 
 
        g = n0 * n1 * (u0 - u1) * (u0 - u1); // 类间方差（少除了n^2） 
 
        if (g > gMax) { // 大于最大类间方差时 
            gMax = g; // 设置最大类间方差 
            thresh = k; // 取最大类间方差时对应的灰度的k就是最佳阈值 
        } 
    } 
 
    return thresh; 
} 

