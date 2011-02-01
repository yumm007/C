#include <stdio.h>
#define MAXLINE 1000

/* str 栈存储当前逆波兰序, line 为从标准读入的自然计算式, temp 为临时栈，用来储存str栈的出栈*/
static char str[MAXLINE], line[MAXLINE], temp[MAXLINE];
static int pstr = 0, pline = 0, ptemp = 0;

void pushstr(char c) {
	str[pstr++] = c;
}
char popstr(void) {
	return (str[--pstr]);
}
void pushtemp(char c) {
	temp[ptemp++] = c;
}
char poptemp(void) {
	return (temp[--ptemp]);
}
void unget_c(void) {
	--pline;
}
char get_c(void) {
	return (line[pline++]);
}
/* 读入一组数字,依次入栈，并且以逗号结尾*/
void readn(void) {
	int c;
	while ((c = get_c()) && c >= '0' && c <= '9') 
		pushstr(c);
	pushstr(',');
	unget_c();
}

void readline(void){
	int c;

	while ((c = getchar()) != '\n')
		line[pline++] = c;
	line[pline] = '\n';
	pline = 0;
}

int main(void)
{
	int c;
	
	readline();

	while ((c = get_c()) != '\n') {
		switch (c){
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				unget_c();
				readn();
				break;
			case '+': case '-':  /* 遇到加减法时,先读入下一组数字,再存入运算符号 */
				readn();
				pushstr(c);
				break;
			case '*': case '/':  /* 遇到乘除法时,现把当前的预算符出栈,存入操作数,再存入乘除符号 */
				pushtemp(popstr());
				readn();
				pushstr(c);
				pushstr(poptemp()); /* 原先的预算符号进栈 */
			default:
				break;
		}
	}
	
	/* 输出逆波兰数序 */
	pushstr('\0');
	printf("%s\n", str);

	return 0;
}
