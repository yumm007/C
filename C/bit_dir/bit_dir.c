#include <stdio.h>
#include <stdint.h>
#include <string.h>

static uint8_t C1[] = 
{
	0,0,0,0,1,0,0,0,0,
	0,0,0,1,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,1,1,1,0,0,0,
	0,0,0,1,1,1,0,0,0,
};

static uint8_t C2[] = 
{
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,1,1,0,0,0,0,
	0,0,0,1,1,0,0,0,0,
	0,0,1,0,1,0,0,0,0,
	0,1,0,0,1,0,0,0,0,
	1,1,1,1,1,1,1,1,1,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
};


static void print_byte_map(int w, int h, const uint8_t * charmap) {
	int i, j;

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			fprintf(stdout, "%s", charmap[ i*w + j] ? "*" : "#");
		}
		fprintf(stdout, "\n");
	}
}

static void print_byte_arr(int w, int h, const uint8_t * charmap) {
	int i, j;

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			fprintf(stdout, "%02X,", charmap[ i*w + j]);
		}
		fprintf(stdout, "\n");
	}
}

static void change_dict(int dict, int w, int h, const uint8_t *src, uint8_t *dst) {
	int i, j;
	
	switch (dict) {
		case 0:
			for (i = 0; i < h; i++) 		//3
				for (j = 0; j < w; j++)   //2
					*dst++ = *src++;
			break;
		case 90:
			for (i = 0; i < h; i++) 		//3
				for (j = 0; j < w; j++)   //2
					dst[j*h + h-1-i] = *src++;
			break;
		case 180:
			for (i = 0; i < h; i++) 		//3
				for (j = 0; j < w; j++)   //2
					dst[(h-1-i)*w + w-1-j] = *src++;
			break;
		case 270:
			for (i = 0; i < h; i++) 		//3
				for (j = 0; j < w; j++)   //2
					dst[(w-1-j)*h + i] = *src++;
			break;
		default:
			break;
	}
}


static uint8_t C3[] = {
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
	0x00,0x00,0x00,
};

static uint8_t C4[] = {
	0x80,0xff,0x01,
	0x80,0xff,0x01,
	0x80,0xff,0x01,
	0x80,0xff,0x01,
	0x80,0xff,0x01,
	0x80,0xff,0x01,
	0x80,0xff,0x01,
	0x80,0xff,0x01,
	0x80,0xff,0x01,
	0x80,0xff,0x01,
	0x80,0xff,0x01,
};

static int bit_stream_write(uint8_t *dst, int bitoffset, const uint8_t *src, int bitn) {
	const uint8_t *p = src;
	int n;
	//将dst的第bitoffset所在的字节开始，复写为src的字节值，最多写bitn个比特
	if (bitoffset % 8) {
		fprintf(stderr, "1: %d->%d,", bitoffset, 8 - bitoffset % 8);
		dst[bitoffset / 8] |= (*src);
		n = 8 - bitoffset % 8;
		bitn -= n;
		bitoffset += n;
		src++;
	}
	
	n = bitn/8;
	if (n) {
		fprintf(stderr, "2:%d->%d,", bitoffset, n*8);
		memcpy(dst + bitoffset/8, src, n);
		bitoffset += n*8;
		bitn = bitn % 8;
		src += n;
	}
	
	//还剩余bitn个bit没有写
	if (bitn) {
		fprintf(stderr, "3:%d->%d", bitoffset, bitn);
		dst[bitoffset / 8] |= *src;
		src++;
	}

	fprintf(stderr,"\n");
	return src - p;
}

static void bit_write(int startx, int starty, int endx, int endy, const uint8_t *src, int line_bit, int *offset, uint8_t *dst) {
	int h, n;
	
	for (h = 0; h < (endy +1 - starty); h++) {
		n = bit_stream_write(dst, (starty+h)*line_bit + startx, src, endx - startx +1);
		src += n; 
	}
}



int main(void) {
	uint8_t buf[1024];

	uint8_t *c = C1;
	/*
	printf("原始图像.\n");
	memset(buf, 0, sizeof(buf));
	change_dict(0, 9, 12, c, buf);
	print_byte_map(9, 12, buf);

	printf("旋转90度.\n");
	memset(buf, 0, sizeof(buf));
	change_dict(90, 9, 12, c, buf);
	print_byte_map(12, 9, buf);

	printf("旋转180度.\n");
	memset(buf, 0, sizeof(buf));
	change_dict(180, 9, 12, c, buf)
	print_byte_map(9, 12, buf);

	printf("旋转270度.\n");
	memset(buf, 0, sizeof(buf));
	change_dict(270, 9, 12, c, buf);
	print_byte_map(12, 9, buf);
	*/
	//printf("按bit写.\n");
	//将C4里的内容复制到C3指定的坐标内
	bit_write(1,0,10,10, C4, 24, 0, C3);
	print_byte_arr(3, 12, C3);

	return 0;
}

