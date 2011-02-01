/*
** 打印MAXNUM以内的质数
** 使用数组存放2-(MAXNUM+2)之间的数，函数结束后返回找到的质数个数max.
** 并且在数组0-(max -1)之间存储找到的质数r.。
** 2010-08-31 yumm_007@163.com
*/


#include <stdio.h>
#define MAXNUM 200

/* 筛选质数函数，传入数字数组和数组的长度,筛选结束后所有非质数都用0表示  */
int findsu(int num[], int len) {
	int i, j, m , n;
	int max = len;

	for (i = 2; i < max / 2; i++) { 
		if (num[i] == 0)	
			for (j = i + i; j < max; j += i) 
				num[j] = 1;
	}

	/* 以每行10个的形式输出结果 */
	for (i = 0, j = 0; i < max; i++) {
		if (num[i] == 0) {
			printf("%d ",i);
			j++;
		}
	}
	printf("\n%d\n",j);

}

int main(void)
{
	int i, j = 1, max, num[MAXNUM] ;
	
	/* 初始化数组  */
	num[0] = num[1] = 1; 	

	printf("\n");

	max = findsu(num, MAXNUM);

	return 0;
}
