#include <stdio.h>
#define MAXLINE 1000

static char str[MAXLINE];
static char opr[4];
static p = 0, popr = 0;

/* 读入一组数字,以逗号结尾 */
void readn(void) {
	int c;
	while ((c = getchar()) && c >= '0' && c <= '9')
		str[p++] = c;
	str[p++] = ',';
	ungetc(c, stdin);
}

int main(void)
{
	int c;;
	
	readn(); /* 先读入一组数字 */
	while ((c = getchar()) != '\n') {
		switch (c){
			case '+': case '-':  /* 遇到加减法时,先读入下一组数字,再存入运算符号 */
				readn();
				str[p++] = c;
				break;
			case '*': case '/':  /* 遇到乘除法时,现把但前的预算符合出栈,存入操作数,再存入乘除符号 */
				opr[popr] = str[--p];
				readn();
				str[p++] = c;
				str[p++] = opr[popr]; /* 原先的预算符号进栈 */
			default:
				break;
		}
	}
	
	/* 输出逆波兰数序 */
	str[p] = '\0';
	printf("%s\n", str);
}
