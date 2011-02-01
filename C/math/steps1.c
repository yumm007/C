/*
** 走楼梯递归程序
** 总共有STEP级楼梯，每次可以走1－S级，问总共有多少级，并打印其走法
** 算法实现：每走一步(n1级楼梯),递归求剩下的n－n1级数楼梯的走法.
** 如果最后n=0,说明采用的走法合格,计数器加1.如果小于1,则说明走法失败.
** 程序返回时,i会加1,说明下一步将采用2级或者3级楼梯的步法.
** 所有可能的步法都会计算一次,只是结果等于0时方法才会被采用.
** 程序使用堆栈存储每一步的走法,如果n=0时,调用打印堆栈函数输出走法.
** 2010-09-02 yumm_007@163.com
*/

#include <stdio.h>
#define STEP 20        			 /* 定义楼梯数  */
#define S 3						 /* 每一步有1-3步走法 */

int stepnum[STEP];				 /* 存储每一步 */
int steps = 0, pstep = 0;		 /* 方案计数器和栈指针*/

void push(int s) {				 /* 入栈和出栈函数*/
	stepnum[pstep++] = s;
}
void pop(void) {
	stepnum[--pstep] = 0;
}
void printstep(void){			 /* 打印堆栈函数 */
	int i;
	for (i = 0; i < STEP && stepnum[i] != 0; i++)
		printf("%d ",stepnum[i]);
	printf("\n");
}

void step(int n) {				/* n级楼梯的递归程序*/
	int i;
	
	if ( n == 0) {				/* n<=0 说明已经走完了楼梯*/
		steps++;
		printstep();
		return;
	} else if ( n < 0)
		return;

	for (i = 1; i <= S; i++) { 	/* 因为下一步有1-3种走法,说明使用了循环*/
		push(i);			   	/* 将下一步的走法入栈,以供打印*/
		step( n - i);
		pop();					/* 从下一步返回,则清空下一步,*/
	}
}

int main(void) {
	
	int n = STEP;

	step(n);	

	printf("Total step %d\n",steps);
}
