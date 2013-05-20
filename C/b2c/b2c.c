#include <stdio.h>

static void tran(FILE *fp) {
	int c, newline = 0;
	while ((c = fgetc(fp)) != EOF) {
		if (newline++ % 16 == 0)
			fprintf(stdout, "\n");
		fprintf(stdout, "0X%02X, ", c);
	}
	fprintf(stdout, "\n};\n");
}

int main(int argc, char **argv) {
	FILE *fp;
	int i;

	if (argc < 2) {
		fprintf(stdout, "const unsigned char b2c[] = {");
		tran(stdin);
		return 0;
	}

	for (i = 1; i < argc; i++) {
		if ((fp = fopen(argv[i],"r")) == NULL) {
			fprintf(stderr, "open file %s failed!\n", argv[i]);
			continue;
		}
		fprintf(stdout, "const unsigned char %s[] = {", argv[i]);
		tran(fp);
	}

	return 0;
}
