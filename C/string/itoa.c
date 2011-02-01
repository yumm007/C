#include <stdio.h>
#include <string.h>

extern reverse(char s[]);

void itoa(char s[], int digit) {
	int i;
	int isneg = 0;

	if (digit < 0) {
		digit = -digit;
		isneg = 1;
	}

	i = 0;
	do {
		s[i++] = digit % 10 + '0';
		digit /= 10;
	} while (digit > 0);

	if (isneg)
		s[i++] = '-';

	s[i] = '\0';

	reverse(s);
}

/* 将字符串s倒置，函数没有返回值 */
int main(void)
{
	char s[32];
	int n = -2147483648;

	itoa(s, n);
	printf("%s\n", s);

	return 0;
}
