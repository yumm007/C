/*
** 逆波兰算术
** 计算10以内的加减乘除取模运算，2+6*2-8+4 表达为 262*+8-4+
** 2010-09-01 yumm_007@163.com
*/
#include <stdio.h>
#define MAXPOINT 1000

/* 栈及其指针  */
static int pnum = 0;
static int num[MAXPOINT];

int numpop(void) {
	return num[--pnum];
}

void numpush(int c) {
	num[pnum++] = c;
}

int main(void)
{
	int c, n = 0, temp1, temp2;

	while ((c = getchar()) != '\n' && pnum <= MAXPOINT && pnum >=0 ) {
		if (c == ',' || c == ' ') {
			numpush(n);
			n = 0;
		}
		else if (c >= '0' && c <= '9')
			n = n * 10 + c - '0';	
		else { 
			temp1 = numpop();
			temp2 = numpop();
			switch (c) {
				default:
					printf("usage: number or symbol is wrong: %c\n",c);
					return -1;
				case '+':
					numpush(temp2 + temp1);
					break;
				case '-':
					numpush(temp2 - temp1);
					break;
				case '*':
					numpush(temp2 * temp1);
					break;
				case '/':
					numpush(temp2 / temp1);
					break;
				case '%':
					numpush(temp2 % temp1);
					break;
			}
		}
	}
	
	printf("%d\n", numpop());
	
	return 0;
}

