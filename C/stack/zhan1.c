#include <stdio.h>
#define MAXPOINT 1000

/* 栈及其指针  */
static int pnum = -1, popr = -1;
static int num[MAXPOINT];
static char opr[MAXPOINT];

int numpop(void) {
	return num[pnum--];
}

void numpush(int c) {
	num[++pnum] = c;
}


char oprpop(void) {
	return opr[popr--];
}

void oprpush(char c) {
	opr[++popr] = c;
}

int main(void)
{
	int c, temp1, temp2;

	while ((c = getchar()) != '\n' && pnum < MAXPOINT && popr < MAXPOINT) {
		if (c >= '0' && c <= '9')
			numpush(c - '0');
		else
			oprpush(c);	
	}
	
	
	while (popr >= 0)
		switch (oprpop()) {
			case '+':
				temp1 = numpop();
				temp2 = numpop();
				numpush(temp2 + temp1);
				break;
			case '-':
				temp1 = numpop();
				temp2 = numpop();
				numpush(temp2 - temp1);
				break;
			default:
				break;
		}
	
	printf("%d\n", numpop());

	return 0;
}

