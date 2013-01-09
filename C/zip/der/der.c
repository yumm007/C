#include <stdio.h>

static void der(const unsigned char *data, int len, FILE *fp) {
	int i, j;
	unsigned char tmp, p = 0;
	//data[0] + 1 is data 
	for (i = data[0] + 1; i < len; i++) {
		tmp = data[i];
		for (j = 7; j >= 0; j--) {
			if (tmp & (1 << j)) {
				fprintf(fp, "%c", data[1+p]);
				p = 0;
			} else {
				p++;	// 找到一个0,不是结束符号
			}
		}
	}
}

int main(int argc, char **argv) {
	FILE *fp;
	unsigned char data[1024];
	int n;

	if ((fp = fopen(argv[1], "r")) == NULL) {
		perror(argv[1]);
		return 1;
	}

	n = fread(data, 1, 1024, fp);

	der(data, n, stdout);

	return 0;
}
