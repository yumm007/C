/*  打印矩形和三角形  
**  程序接受两个参数，高度和宽度。
**  当输入3，7时候，打印格式如下（分两行显示）
**
**	*******          A
**  *******         ABA
**  *******        ABCBA
**
**  2010-08-30 yumm_007@163.com
*/

#include <stdio.h>

void print1(int high, int weight){
	
	int i, j;
	for (i = 1; i <= high; i++){
		for (j = 1; j <= weight; j++)
			putchar('*');
		putchar('\n');
	}

	return;
}

/*打印三角形，mid为中间线，在mid+－i的范围内打印字母，其余的地方打印空格*/
void print2(int high){
	
	int i, j, c;
	int mid = (high * 2 - 1) / 2 + 1;

	for (i=0; i <= high - 1; i++){
		c = 'A';
		for (j=1; j <= high * 2 -1; j++){
			if (j < mid - i || j > mid + i)
				putchar(' ');
			else if ( j >= mid -i && j < mid)
				putchar(c++);
			else
				putchar(c--);
		}	
		putchar('\n');
	}

	return;
}


/*字符串转数字，str是指向存储数字的数组.当含有非数字字符时返回－1*/
int ston(const char *str){
	
	int n = 0; 		/*记录返回的数字*/

	while ( *str )
		if ( *str < '0' || *str > '9' ) /*当前字符为非数字时返回-1*/
			return -1;
		else
			n = n * 10 + *(str++) - '0';

	return n;
}

int main(int argc, char *argv[]){
	
    int high, weight;

	if (argc != 3) {
		printf("usage: print12 high weight\n");
		return -1;
	}

	/*如果函数返回－1说明输入的参数格式不对，退出程序*/
	if ((high = ston(argv[1])) == -1 || high > 26 || \
		(weight = ston(argv[2])) == -1) {
		printf("high=%s or weight=%s is not valid number!\n",\
				argv[1],argv[2]);
		return -1;
	}
	
	print1(high, weight);	
	print2(high);

	return 0;
}

