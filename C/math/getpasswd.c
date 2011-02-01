/*
** 使用递归穷举破解密码
** charc[]为密码可能使用的字符库,charlen为它包含的字符数
** 2010-09-02 yumm_007@163.com
*/


#include <stdio.h>
#include <string.h>
#define MAXNUM 255			/* 需要破解的密码的最大长度 */
#define MAXLEVER 20			/* 尝试破解的最大长度*/

static char charc[]="1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+=,.<>?";
static char temp[MAXNUM];			/* 临时存储密码的字符串. charlen为密码字符集长度,通过strlen(charc)取值 */
static int ptemp = 0, flag = 0, charlen;		/*  ptemp为临时密码的位置栈指针, flag=1说明已经找到密码,会通知所有循环立即退出 */

#if 0
void push(char c){
	temp[ptemp++] = c;
}
void pop(void){
	--ptemp;
}
#endif

void getpasswd(const char pass[], int len){
	int i;

	if (len == 0) {                   				/* len = 0, 说明找到符合指定长度的字符串  */  
		temp[ptemp] = '\0';
		if (strcmp(temp,pass) == 0) { 				/* 再拿它和密码字符串对比,如果相同,则打印并置flag=1*/
			printf("the word is: %s\n", temp);		
			flag = 1;
		}
		return;										/* 程序返回 */
	}

	for (i = 0; i< charlen && flag == 0; i++) {
		temp[ptemp++] = charc[i];							/* 不使用栈函数*/
	//	push(charc[i]);										/* 看是否能压缩时间*/
		getpasswd(pass, len -1);
		--ptemp;
	//	pop();		
	}
}

int main(void)
{
	char pass[MAXNUM];
	int c, i = 0;
	charlen = strlen(charc);						/* 获取密码字符集长度 */

	while ((c = getchar()) != '\n')					/* 获取传入密码 */
			pass[i++] = c;
	pass[i] = '\0';

	for (i = 1; i <= MAXLEVER && flag == 0; i++)    /* 循环调用递归函数分别查找1-N位数密码 */
		getpasswd(pass, i);

	return 0;
}
