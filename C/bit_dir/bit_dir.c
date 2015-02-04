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


static void print_bit_map(int w, int h, const uint8_t * charmap) {
	int i, j;

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			fprintf(stdout, "%s", charmap[ i*w + j] ? "*" : "_");
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


int main(void) {
	uint8_t buf[1024];

	uint8_t *c = C1;

	printf("原始图像.\n");
	memset(buf, 0, sizeof(buf));
	change_dict(0, 9, 12, c, buf);
	print_bit_map(9, 12, buf);

	printf("旋转90度.\n");
	memset(buf, 0, sizeof(buf));
	change_dict(90, 9, 12, c, buf);
	print_bit_map(12, 9, buf);

	printf("旋转180度.\n");
	memset(buf, 0, sizeof(buf));
	change_dict(180, 9, 12, c, buf);
	print_bit_map(9, 12, buf);

	printf("旋转270度.\n");
	memset(buf, 0, sizeof(buf));
	change_dict(270, 9, 12, c, buf);
	print_bit_map(12, 9, buf);

	return 0;
}

