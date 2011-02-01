/*
** 函数reverse 实现倒置传入的字符串功能
** 2010-08-31 yumm_007@163.com
*/

#include <string.h>

/* 将字符串s倒置，函数没有返回值 */
void reverse(char s[]) {
	int i, len, c;

	/* i和len为要交换的两个字母在字符串中的位置，每次交换后他们的值增1和减1 */
	for (i = 0, len = strlen(s) - 1; i < len; i++, len--) {
		c = s[i];
		s[i] = s[len];
		s[len] = c;
	}
}


