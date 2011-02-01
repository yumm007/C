#include <stdio.h>

int main(void)
{
	char str1[] = "test string";
	char *str2[] = {"abcd efghig", "str2", "str3"};
	
	const char *p1  = str1;
	char * const p2 = str2;

	*(p1+2) = 'D';
	str2[2] = 'D';
	*(p2+5) = '\0';	
	p2 = p1;

	printf("%s\n", p1);
	printf("%s\n", str2);

	return 0;
}
