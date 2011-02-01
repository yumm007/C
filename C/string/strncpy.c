#include <stdio.h>
#define MAXLINE 1024

#if 1
int mystrncmp(char *dest, char *src, int n) {
	while ((*dest++ == *src++) && *dest != 0 && n--)
		;
	return (*--dest - *--src);
}
#else
int mystrncmp(char *dest, char *src, int n) {
	int i;
	
	for (i = 0; dest[i] == src[i] && dest[i] != '\0' && n--; i++)
		;
	return (dest[i] - src[i]);
}
#endif

void mystrncpy(char *dest, char *src, int n) {
	while ( n-- && (*dest++ = *src++))
		;
	if (n == 0)
		*dest = '\0';
}

void mystrncat(char *dest, char *src, int n){
	while (*dest++) {
		;
	}
	dest--;
	while (n-- && (*dest++ = *src++))
		;
}

int main(void) {
	char *templine = "35eg";
	char dest[MAXLINE];
	int n = 1;	
	mystrncpy(dest, templine, n);
	printf("%s\n",dest);
	//mystrncat(dest, templine, n);
	//printf("%s\n",dest);
	//printf("%d\n",mystrncmp(dest,templine, n));

	return 0;
}
