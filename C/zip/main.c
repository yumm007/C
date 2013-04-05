#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const unsigned char gImage_master1[3096];
extern const unsigned char gImage_master2[3096];
extern const unsigned char gImage_master3[3096];
extern const unsigned char gImage_master4[3096];
extern const unsigned char gImage_master5[3096];
extern const unsigned char gImage_master6[3096];


static void arr_to_file(const unsigned char *buf, int buf_size, const char *file_name) {
	FILE *fp;

	if ((fp = fopen(file_name, "w+")) == NULL) {
		perror(file_name);
		return;
	}

	fwrite(buf, 1, buf_size, fp);
	
	fclose(fp);
}

struct count_t {
	unsigned char v;
	int count;
};

static int cmp_fun(const void *a, const void *b) {
	return  ((struct count_t *)b)->count - ((struct count_t *)a)->count;
}


static int offset;
static unsigned char byte;

static void flush_bit(FILE *fp) {
	//for (; offset < 8; offset++)
	//	byte <<= 1;
	fprintf(fp, "%c", byte);
	offset = 0;
	byte = 0;
}

static void input_bit(int b, FILE *fp) {
	offset++;
	byte <<= 1;
	byte |=  (b ?  1: 0);

	if (offset == 8) {
		fprintf(fp, "%c", byte);
		offset = 0;
		byte = 0;
	}
}


static void count_byte(const unsigned char *buf, int buf_size) {
	struct count_t count_arr[256];
	int i, j;
	unsigned char ff[256];

	for (i = 0; i < 256; i++) {
		count_arr[i].v = i;
		count_arr[i].count = 0;
	}
	memset(ff, 0, 256);

	for (i = 0; i < buf_size; i++)
		count_arr[buf[i]].count++;

	qsort(count_arr, sizeof(count_arr) / sizeof(struct count_t), sizeof(struct count_t), cmp_fun);
	for (i = 0; count_arr[i].count != 0; i++) 
		;
	fprintf(stdout, "%c", i);	//总共有i个压缩的字节
	for (i = 0; count_arr[i].count != 0; i++) {
		fprintf(stdout, "%c", count_arr[i].v);	//按顺序分别如下
		ff[count_arr[i].v] = i;
	}
	
	//1,01,001,0001,
	for (i = 0; i < buf_size; i++) {
		fprintf(stderr,"input %02x, will store %d\n", buf[i], ff[buf[i]]);
		for (j = 0; j < ff[buf[i]]; j++)
			input_bit(0, stdout);
		input_bit(1, stdout);
	}
	flush_bit(stdout);
}


int main(void) {

	count_byte(gImage_master3, 3096);
	#if 0
	printf("\n------------------\n");
	count_byte(gImage_master2, 3096);
	printf("\n------------------\n");
	count_byte(gImage_master3, 3096);
	printf("\n------------------\n");
	count_byte(gImage_master4, 3096);
	printf("\n------------------\n");
	count_byte(gImage_master5, 3096);
	printf("\n------------------\n");
	#endif
	return 0;
}
