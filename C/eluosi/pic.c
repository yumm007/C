#include "pic.h"

static int init_bmp(BMP_FH *bmp_fh, BMP_FI *bmp_fi, FILE *fp) {

	if (fread(bmp_fh, sizeof(*bmp_fh), 1, fp) < 1 || \
		fread(bmp_fi, sizeof(*bmp_fi), 1, fp) < 1 ) 
		return -1;

	if (bmp_fh->bfType != BMPFILE)	
		return -1;

	fseek(fp, bmp_fh->bfOffBits, SEEK_SET);

	return 0;
}

static void read_color(struct color_v *color, int bpp, FILE *fp) {
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
}

static void draw_pic(BMP_FI *bmp_fi, FILE *fp, int x, int y) {
	int i, j, bpp, offset;
	struct color_v color = {0,0,0,0};

	bpp = bmp_fi->biBitCount / 8;
	offset = (4 - (bmp_fi->biWidth * bpp) % 4) % 4;

	for (i = bmp_fi->biHeight; i > 0; i--) {
		for (j = 0; j < bmp_fi->biWidth; j++) {
			read_color(&color, bpp, fp);
			fb_draw_point_v2(x+j, y+i, &color);	
		}
		for (j = offset; j > 0; j--)
			fgetc(fp);
	}

}

int background_pic(char *filepath) {
	BMP_FH bmp_fh;		
	BMP_FI bmp_fi;
	FILE *fp;
	
	if ((fp = fopen(filepath,"r")) == NULL)
		return -1;

	if (init_bmp(&bmp_fh, &bmp_fi, fp) == -1) {
		fclose(fp);
		return -1;
	} 

	draw_pic(&bmp_fi, fp, 0, 0);

	fclose(fp);
   return 0;
}
