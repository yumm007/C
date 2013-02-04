#include <stdio.h>

#define PIC_ROW_COUNT	10
#define PIC_ROW_BYTE		21
#define BUF_SIZE			PIC_ROW_COUNT * PIC_ROW_BYTE

static int *min_x, *min_y, *max_x, *max_y;

static void get_max_xy(const char *data) {
	min_x = (int *)&data[sizeof(int) * 0];
	min_y = (int *)&data[sizeof(int) * 1];
	max_x = (int *)&data[sizeof(int) * 2];
	max_y = (int *)&data[sizeof(int) * 3];
	fprintf(stderr, "min(%d, %d) max(%d, %d)\n", *min_x, *min_y, *max_x, *max_y);
}

//fp不支持往回写，只能追加写
static void output_and_merger_diff(FILE *fp, \
			const char *prov_buf, const char *diff_buf) 
{
		int y, i, y_off, l1, l2,l3;

		//先写头min_y行
		for (y = 0, y_off = 0; y < *min_y; y++, y_off += PIC_ROW_BYTE)
			fwrite(&prov_buf[y_off], 1, PIC_ROW_BYTE, fp);

		//中间部分共max_y - min_y+1行，每行从prov_buf取前l1个字节，
		//从diff_buf取l2个字节,再从prov_buf中取l3个字节
		l1 = *min_x;
		l2 = *max_x - *min_x + 1;
		l3 = PIC_ROW_BYTE - *max_x - 1;
		for (y = *min_y, i = 0; y <= *max_y; y++, y_off += PIC_ROW_BYTE, i += l2) {
			fwrite(&prov_buf[y_off], 1, l1, fp);
			fwrite(&diff_buf[i], 1, l2, fp);
			fwrite(&prov_buf[y_off + l1 + l2], 1, l3, fp);
		}
		
		//最后写max_y - PIC_ROW_COUNT行
		for (y = *max_y + 1; y < PIC_ROW_COUNT; y++)
			fwrite(&prov_buf[y * PIC_ROW_BYTE], 1, PIC_ROW_BYTE, fp);
}

int main(int argc, char **argv) {
	FILE *diff_fp, *prov_fp;
	char diff_buf[BUF_SIZE], prov_buf[BUF_SIZE];
	int ret = -1;

	if (argc != 3) {
		fprintf(stderr, "%s old_file diff_file\n", argv[0]);
		goto _exit;
	}

	if ((prov_fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "open file %s failed.\n", argv[1]);
		goto _close_diff_fp;
	}
	if ((diff_fp = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "open file %s failed.\n", argv[2]);
		goto _exit;
	}
	fread(diff_buf, 1, BUF_SIZE, diff_fp);
	fread(prov_buf, 1, BUF_SIZE, prov_fp);

	get_max_xy(diff_buf);
	output_and_merger_diff(stdout, prov_buf, &diff_buf[sizeof(int)*4]);

	ret = 0;
	fclose(prov_fp);
_close_diff_fp:
	fclose(diff_fp);
_exit:
	return ret;
}
