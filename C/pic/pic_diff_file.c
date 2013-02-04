#include <stdio.h>
#include <stdbool.h>

#define PIC_ROW_COUNT	10	//屏幕的行数
#define PIC_ROW_BYTE		21	//每行的字节数
#define PIC_BYTE_COUNT	PIC_ROW_COUNT*PIC_ROW_BYTE

static int min_x, min_y, max_x, max_y;

//输入每个不同点的坐标，重新界定不同处的方框坐标
static void update_xy(int x, int y) {
	static bool first_diff_point = true;

	if (first_diff_point) {
		min_x = max_x = x;
		min_y = max_y = y;
		first_diff_point = false;
	} else {
		if (x < min_x)
			min_x = x;
		else if (x > max_x)
			max_x = x;

		if (y < min_y)
			min_y = y;
		else if (y > max_y)
			max_y = y;
	}

	fprintf(stderr, "update_xy: min(%d,%d), max(%d, %d)\n", \
			min_x, min_y, max_x, max_y);
}

//根据当前字节在文件流中的偏离量，算处其在方框中的坐标
//偏移量除以每行字节数的商为y，余数为x
static void get_xy(int p, int *x, int *y) {
	*y = p / PIC_ROW_BYTE;
	*x = p % PIC_ROW_BYTE;
	fprintf(stderr, "point %d(%d, %d)\n", p, *x, *y);
}

int main(int argc, char **argv) {
	FILE *fp1, *fp2;
	char BUF1[PIC_BYTE_COUNT], BUF2[PIC_BYTE_COUNT];
	int ret = -1, i, x, y;

	if (argc != 3) {
		fprintf(stderr, "%s file1 fil2\n", argv[0]);
		goto _exit;
	}

	if ((fp1 = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "open %s failed.\n", argv[1]);
		goto _exit;
	}
	if ((fp2 = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "open %s failed.\n", argv[2]);
		goto _close_fp1;
	}

	fread(BUF1, PIC_BYTE_COUNT, 1, fp1);
	fread(BUF2, PIC_BYTE_COUNT, 1, fp2);

	for (i = 0; i < PIC_BYTE_COUNT; i++) {
		if (BUF1[i] == BUF2[i])
			continue;
		get_xy(i, &x, &y);
		update_xy(x, y);
	}


	fclose(fp2);
_close_fp1:
	fclose(fp1);
_exit:
	return ret;
}
