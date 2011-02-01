#include <stdio.h>
#include <string.h>
#define __USE_XOPEN
#include <unistd.h>

#define CODELEN 9

static int charlen, codelen, flag = 1;
static const char code[] = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+=,.<>?";
static char salt[11], passwd[34];

static int __decode(char buf[], int ind)
{
    int i;
    char *p;

	if (ind == codelen) {
		buf[ind] = '\0';
		p = crypt(buf, salt);
      	if (strcmp(p, passwd) == 0) {
    		printf("%s\n", buf);
			flag = 0;
		} 
		return;
	}

    for (i = 0; i < charlen && flag; i++) {
        buf[ind] = code[i];
        __decode(buf, ind + 1);
      }
}

void decode(void)
{
	charlen = strlen(code);

	for (codelen = 1; codelen <= CODELEN && flag; codelen++) {
       char buf[codelen + 1 + 8];
       __decode(buf, 0);
	}
}

void readmid5pw(void){
	int i = 0, j = 0, c;
	
	while ((c = getchar()) != ':')
		;
	while ((c = getchar()) != '\n' && j < 34) {
		if (i < 11)
			salt[i++] = c;
		if (j < 34)
			passwd[j++] = c;
	}
	salt[i] = '\0';
	passwd[j] = '\0';
}

int main(void)
{
	  readmid5pw();
      decode();
      return 0;
}

