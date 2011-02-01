#include <stdio.h>
#include <stdlib.h>

#define	BITLEN 	256
#define	Q_FULL 	-1
#define	Q_EMPTY 	-2

struct let{
	int c;
	unsigned num;
};

static int bitqueue[BITLEN], en_p, de_p;

static int cmp(const void *num1, const void *num2) {
	return ((struct let*)num1)->num - ((struct let*)num2)->num;
}

static void printbit(unsigned int n) {
   int i;
   for (i = 7; i >= 0; i-- ) { 
      putchar((n >> i & 1) + '0');
      n = ~(1 << i) & n;
   }   
}

static void flush_q(void) {
	unsigned i, n = 0;

	for (i = 0; i < en_p ; i++) {
		n = (n << 1) | bitqueue[i];
		if (i % 8 == 7) {
			putchar(n);
			n = 0;
		}
	}

	en_p = 0;
}

static void __en_queue(int bit) {
	if (en_p == BITLEN) //Q_FULL
		flush_q();
	bitqueue[en_p++] = bit;
}

static void en_queue(int c, int *letdec) {
	int i, n;
	for (i = 1, n = letdec[c]; i <= n; i++)
		__en_queue(0);
	__en_queue(1);
}

void comp(FILE *fp, int *letdec) {
	int c, i;
	
	while ((c = fgetc(fp)) != EOF) 
		en_queue(c, letdec);

	for (i = 8 - (en_p - (en_p / 8) * 8); i >= 1; i--)
		__en_queue(0);

	flush_q();
}

static void init_letdec(FILE *fp, struct let *l, int *letdec) {
	int i, j, c;

	for (i = 0; i < BITLEN; i++) {
		l[i].c = i;
		l[i].num = 0;
		bitqueue[i] = 0;
	}

	en_p = de_p = 0;

	while ((c = fgetc(fp)) != EOF)
		l[c].num++;
	rewind(fp);
	
	qsort(l, BITLEN, sizeof(struct let), cmp);

	for (i = BITLEN - 1, j = 0; i >= 0; i--, j++)
		letdec[l[i].c] = j;

}


int main(int argc, char *argv[])
{
	struct let l[BITLEN];
	int letdec[BITLEN];
	int i;
	FILE *fp;

	if (argc != 2)
		return -1;

	if ((fp = fopen(argv[1], "r")) == NULL)
		return -1;

	init_letdec(fp, l, letdec);

	comp(fp, letdec);

/*
	for (i = 0; i < BITLEN; i++) {
		printf("let[%c,%u,", l[i].c, l[i].c);
		printbit(l[i].c);
		printf("] = %u\t",l[i].num);
		if (i % 4 == 0)
			printf("\n");
	}
	printf("\n---------- letdec -------------\n");

	for (i = 0; i < BITLEN; i++) {
		printf("letdec[%u, %c, %u]\t", i, i, letdec[i]);
		if (i % 4 == 0)
			printf("\n");
	}

	printf("\n");
*/

	return 0;
}
