#include <stdio.h>
#include <string.h>
#define MAXLINE 1024

char *mystrncpy(char *dest, const char *src, int n) {
	int i, len = strlen(dest);

	for (i = 0; i< n && (dest[i] = src[i]); i++)
		;
	for (;i <= len ; i++)
		dest[i] = '\0';
	
	return dest;
}
#if 0
char *mystrncat(char *dest, const char *src, int n) {

	int i;
	char *p = dest;

	while (*dest++)
		;
	dest--;
	for ( i = 0; i < n && src[i]; i++)
		*dest++ = src[i];
	*dest = '\0';
}

#else

char *mystrncat(char *dest, const char *src, int n) {
	int i, len = strlen(dest);
	for (i = 0 ; i < n && src[i] != '\0' ; i++)
		dest[len + i] = src[i];
	dest[len + i] = '\0';

	return dest;
}
#endif

int mystrncmp(char const *stra, char const *strb, int n) {
	int i = 0;
	if (n == 0)
			return 0;
	while ( n-- && (stra[i] == strb[i]))
		i++;
	return stra[i] - strb[i];
}


int main(void)
{
	char const temp[] = "aaa";
	char dest1[MAXLINE] = "bbbb";
	char dest2[MAXLINE] = "cccc";
	int n = 1;
	
	mystrncpy(dest1, temp, n);
	printf("%s, len of dest1: %d\n", dest1, strlen(dest1));
	mystrncat(dest2, temp, n);
	printf("%s, len of dest2 %d\n", dest2, strlen(dest2));
	printf("%s - %s = %d\n", dest1, dest2, mystrncmp(dest1, dest2, n));

	return 0;
}
