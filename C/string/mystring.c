/*
** 部分string.h函数
** 2010-09-15 yumm_007@163.com
*/

#include <stdio.h>
#include <stdlib.h>

#define REALLOWSIZE 240

static char *__strchr(const char *s, int c) {
	int i;
	
	for (i = 0; s[i] != c && s[i] != '\0'; i++)
		;
	if (s[i] == '\0')
		return NULL;
	return (char *)(s+i);
}

static char *__strstr(const char *str, const char *key) {
	int i, j, k;
	i = j = k = 0;

	while (str[i] != '\0') {
		if (str[i++] != key[0])
			continue;
		for (j = i, k = 1; key[k] != '\0' && str[j] == key[k]; j++, k++)			
			;
		if (key[k] == '\0')
			return (char *)(str + i - 1);
	}
	return NULL;
}

static char *__strtok(char *str, const char *delim) {
	static char *ret, *start;
	int i; 
	
	if (str != NULL)
		start = str;

	ret = start; /* ret记录本此调用时字符指针的位置 */
	for (i = 0; start[i] != '\0' && __strchr(delim, start[i]) == NULL; i++) 
			; /*找到一个分界符*/

	if (ret == &start[i]) /* 如果没有挪动位置，说明已经到了末尾*/
		return NULL;
	
	if (start[i] == '\0') { /* 如果在最后字段没有找到分隔符*/
		start = &start[i];
		return ret;
	}

	start[i++] = '\0'; /*将此位置0*/

	for (; start[i] != '\0' && __strchr(delim, start[i]) != NULL; i++) 
			; /*找到一个非分界符*/

	start = &start[i]; /* 将此位置设置为下次进入时的start值 */
	return ret;
}

static void *__memcpy(void *dest, const void *src, size_t n) {
	char *pdest, *psrc;
	for (pdest = (char *)dest, psrc = (char *)src; n > 0 ;n--)
		*pdest++ = *psrc++;
	return dest;
}

static void *__memmove(void *dest, const void *src, size_t n) {
	char *pdest, *psrc;
	for (pdest = (char *)dest+n-1, psrc = (char *)src+n-1; n > 0 ;n--)
		*pdest-- = *psrc--;
	return dest;
}

static ssize_t __getline(char **lineptr, size_t *n, FILE *stream){
	int step, c, allowsize;
	ssize_t i = 0;
	char *ptr = NULL;
	
	//决定每次申请的空间大小
	if (*n <= 0)
		*n = REALLOWSIZE;
	step = *n;
	allowsize = step;

	if (*lineptr == NULL){  /* 如果传入的是空指针 */
		if ((void *)(ptr = malloc(allowsize)) == NULL)
			return -1;
	}else 
		ptr = *lineptr;

	while ((c = fgetc(stream)) != EOF){
		if ((ptr[i++] = c) == '\n')
			break;
		if (i == allowsize - 1) /*留一个空间写尾0*/
			if ((void *)(ptr = realloc((void *)ptr, allowsize +=step)) == NULL)
				return -1;
	}

	if (c == EOF) { /* 如果读到EOF并且是本函数申请的空间，把它free掉 */
		if (*lineptr == NULL)
			free(ptr);
		return -1;	
	}	

	ptr[i] = '\0';
	*n = allowsize;
	*lineptr = ptr;
	return i;
}

int main(int argc, char *argv[])
{
	char *s = NULL;
	char str[] = "1desk: /2desk:	3desk:4desk   5desk	dd 6 past";
	char st4[] = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";
	char src[] = "past to string";
	//char str[] = "1desk";
	char delim[] = " /:\t";
	size_t i = 10;
	int p;

#if 0

	s = __strchr("desk", 's');
	if (s != NULL)
		printf("%s\n",s);

	s = __strstr("deskdesk", "es");
	if (s != NULL)
		printf("%s\n",s);

	s = __strtok(str, delim);
	do {	
		printf("%s\n",s);
		s = __strtok(NULL,delim);
	} while (s != NULL);
	__memmove(src, str, 15);
	printf("%s\n",src);

#endif
	
	p = __getline(&s, &i, stdin);
	printf("%sread char %d, buf size = %d\n",s,p, i);

	i = 0;
	while(s[i] != '\0')
		printf("%d ",s[i++]);
	printf("%d\n",s[i]);
	
	free(s);

	return 0;
}
