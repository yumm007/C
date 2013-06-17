#include <stdio.h>

#define    ASC_12_OFFS  0
#define    ASC_16_OFFS   (ASC_12_OFFS + 1536)
#define    ASC_24_OFFS   (ASC_16_OFFS + 4096)
#define    HZK_16_OFFS   (ASC_24_OFFS + 12288)
#define    HZK_24_OFFS   (HZK_16_OFFS + 267616)
#define    HZK_14_OFFS   (784064) //size = 189504
#define    ASC_14_OFFS   (HZK_14_OFFS + 189504)
#define    HZK_12_OFFS   974898 //新宋 9号字体，12x12，size = 162432

typedef struct font_t {
	const char *file_name;
	int start_addr;
} font_t;

static font_t font_arr[] = {
   {"songti_asc_12.bin",   ASC_12_OFFS},      //ASC_12
   {"songti_asc_16.bin",   ASC_16_OFFS},      //ASC_16
   {"songti_asc_24.bin",   ASC_24_OFFS},     //ASC_24
   {"songti_hz_16.bin",  	HZK_16_OFFS},      //ASC_14
   {"songti_hz_24.bin",    HZK_24_OFFS},     //HZK_16
   {"songti_hz_14.bin",    HZK_14_OFFS},     //HZK_24
   {"songti_asc_14.bin",   ASC_14_OFFS},     //HZK_14
   {"xingsong_hz_12.bin",  HZK_12_OFFS},     //新宋12号字体

   {NULL,						HZK_12_OFFS + 162432},     //error

};

int main(void) {
	FILE *fp;
	char buf[2048*1024];
	font_t *p = font_arr;

	fp = fopen("font.bin", "r");
	fread(buf, 1, sizeof(buf), fp);
	fclose(fp);

	while (p->file_name != NULL) {
		fp = fopen(p->file_name, "w");
		fwrite(buf + p->start_addr, 1, (p+1)->start_addr - p->start_addr, fp);
		fclose(fp);
		p++;
	}

	return 0;
}
