/*
** 打印MAXNUM以内的质数
** 使用数组存放2-(MAXNUM+2)之间的数，在调用函数结束后，所有非质数用0代替
** 最后打印数组中所有非0的数字
** 2010-08-31 yumm_007@163.com
*/


#include <stdio.h>
#define MAXNUM 1000000

/* 筛选质数函数，传入数字数组和数组的长度,筛选结束后所有非质数都用0表示  */
void findsu(int num[], int len) {
	int i, j, m , n;
	int start = 0, end = len;
	/* 从当前非0的数开始大循环，把它后面每一个非0的数都除一遍，如果余数为0
	** 则把那个数设为0，再继续大循环。  
	*/
	for (i = 0; i < end; i++) { 
		if (num[i] != 0)
			for (j = i + 1; j < end; j++) 
				if (num[j] != 0 && num[j] % num[i] == 0)
					num[j] = 0;

		/* 清除数组中的0 
		start = i ; 
		for (m = end -1; m >= start; m--)
			if(num[m] != 0)
				for (n = start; n < end; n++) {
					if (num[n] == 0) {
						num[n] = num[m];
						start = n + 1;
						end = m;
						break;
					}
				}
	 */	
	}
}

/* 另外一种算法 使用两个数组，每次把排除剩下的数存入另外一个数组
void findsu1(int max) {
	int i, max, end = 0, num1[max], num2[max / 2 + 1];

	 初始化数组1,里面都是奇数 
    num[0] = 2;
	for (i = 1; i < max / 2, i++)
		num2[end++] = 2 * i + 1;

    
	for ( j = )
}
*/

int main(void)
{
	int i, j = 1, num[MAXNUM];
	
	/* 初始化数组  */
	for (i = 0; i < MAXNUM; i++)
		num[i] = i + 2;

	findsu(num, MAXNUM);

	/* 以每行10个的形式输出结果 */
	for (i = 0; i < MAXNUM; i++)
		if (num[i] != 0) {
			printf("%d", num[i]);
			(j++ % 10) ? printf("\t") : printf("\n");
		}
	printf("\n");

	return 0;
}
