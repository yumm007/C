#include <stdio.h>
#define MAXLINE 1024


int getline(char templine[]) {
	int c, tempnum = 0, i = 0;

	while ((c = getchar()) != '\n') {
		if (c == EOF)
			return 0;
		if (tempnum++ < MAXLINE -1) { 
			templine[i++] = c;
		}
	}
	templine[i++] = '\n';
	templine[i] = '\0';

	return i;
}

void copystr(const char from[], char to[]) {
	while ( (*to++ = *from++) != '\0' )
		;
}


int main() {

	char templine[MAXLINE], maxline[MAXLINE];
	int tempmax = 0, maxnum = 0;
	
	while ( (tempmax = getline(templine)) != 0 ) {
			if (tempmax > maxnum) {
				maxnum = tempmax;
				copystr(templine, maxline);
			}
	}
	
	printf("%d\t%s", maxnum, maxline);
	return 0;
}
