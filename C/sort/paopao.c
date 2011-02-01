/*
** 多个排序法
** 2010-08-30 yumm_007@163.com
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 11
#define swap(a, b, temp) { (temp)=(a); (a)=(b); (b)=(temp); }

static int cmp(const void *data1, const void  *data2) {
	return *(int *)data1 - *(int *)data2;
}

/* 选择排序法，大循环执行N次，小循环用来寻找最大的数。一轮查找结束后执行0次或1次替换 */
void sort(int num[], int len) {
	int i, j, max, temp;
	
	for (i = 0; i < len; i++) {
		max = 0;
		for (j =  1; j <= len - i; j++) {
			if (num[max] < num[j])
				max = j;
		}
		if (max != j)
			swap(num[max], num[len - i], temp);	
	}
}

/* 冒泡排序法，每次相邻的两个数比较，如果前面前的比后面的大,交换他们的位置 */
void sort1(int num[], int len) {
	int i, j, temp;
	int flag = 0;  /* 用来判断是否发生了替换，如果一次替换都没有，则表示数组已经排好序  */

	for (i = 0; i < len; i++) {
		for (j = 0; j < len - i; j++)
			if (num[j] > num[j+1]) {
				swap(num[j], num[j+1], temp);
				if (flag == 0)      /* 发生了替换，将状态位标记为1  */
					flag = 1;
			}

		if (flag == 0) /* 执行一次循环后，状态位未变，退出for循环  */
			break;
	}
}

int main(void)
{
	int i, num[MAX] = {4,3,2,9,5,8,77,6,67,1,3};
	
	//sort1(num, MAX);
	qsort(num, MAX, sizeof(num)/MAX, cmp);

	for (i = 0; i < MAX; i++)
		printf("%d ",num[i]);
	printf("\n");

	return 0;
} 
