/*
** 打印MAXNUM以内的质数
** 使用数组存放2-(MAXNUM+2)之间的数，函数结束后返回找到的质数个数max.
** 并且在数组0-(max -1)之间存储找到的质数r.。
** 2010-08-31 yumm_007@163.com
*/


#include <stdio.h>
#include <math.h>
#define MAXNUM 2000000

/* 筛选质数函数，传入数字数组和数组的长度,筛选结束后所有非质数都用0表示  */
void findsu(int num[], int len) {
	int i, j;
	int max=sqrt(len);

	for (i = 2; i < max + 1; i++) 
		if (num[i])
			for (j = i + i; j < len; j += i) 
				num[j] = 0;
}

int main(void)
{
	int i, j = 1, max, num[MAXNUM];
	
	/* 初始化数组  */
	for (i = 0; i < MAXNUM; i++)
		num[i] = i;
	
	findsu(num, MAXNUM);

	/* 以每行10个的形式输出结果 */
	for (i = 2; i < MAXNUM; i++)
		if (num[i]) {
			printf("%d", num[i]);
			(j++ % 10) ? printf("\t") : printf("\n");
		}
	printf("\n");

	return 0;
}
