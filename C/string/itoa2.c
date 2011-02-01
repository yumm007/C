#include <stdio.h>
void reverse(char s[]) {
	short int i, len, c;

	for (len = 0; s[len] != '\0'; len++)
		;

	/*最后一位'\0' 不参与掉换*/	
	len--;
	for(i = 0; i < len; i++, len--) {
		c = s[i];
		s[i] = s[len];
		s[len] = c;
	}
}

void itoa(short int n, char num[]){
	short int i = 0, temp = n;
	
	/* 倒着存储每一位数，并在最后加'\0' */
	/* 如果是负数，则把他和－10取模和商，得到正数再继续取第2位数 */
	if (n < 0){  
		num[i++] = -(n % -10) + '0';
		n /= -10;
	}
	
	do {
		num[i++] = n % 10 + '0';
		n /= 10;
	} while (n > 0);
	
	if (temp < 0)
		num[i++] = '-';

	num[i] = '\0';
	reverse(num);  /* 逆转字符串函数 */
}

int main(void) {
	char num[31];
	short int n = -32768;	

	itoa(n, num);
	
	printf("%s\n", num);
}
