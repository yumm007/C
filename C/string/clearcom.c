#include <stdio.h>
#define YES 1
#define NO 0


/*
** 删除注释功能
*/

static void cleancom(FILE *stream) {
	int c, nextc, nextcc, nextccc, linecount = 1;
	int printc = YES;

	while ((c=fgetc(stream)) != EOF) {

//删除“//”格式的注释
		if (c == '/')
			if ((nextc = fgetc(stream)) == '/') {
				while(getchar() != '\n')
					;
				if (linecount > 1)
					putchar('\n');
				continue;
			} else	
				ungetc(nextc, stream);
/* 删除注释功能 */
// 删除注释功能
#if 1
		if (c == '/')
			if ((nextc = fgetc(stream)) == '*') {
				while(nextcc = fgetc(stream))
					if (nextcc = '*' && (nextccc = fgetc(stream)) == '/')
						break;
					else
						ungetc(nextccc, stream);
				printc = YES;
				continue;
			} else
				ungetc(nextc, stream);

#else
		if (c == '/')
			if ((nextc = fgetc(stream)) == '*') {
				printc = NO;
				continue;     // 删除注释功能
			} else
				ungetc(nextc, stream);

		if ( c == '*')
			if ((nextc = fgetc(stream)) == '/') { /*当一行只有注释和
回车符时,忽略回车符
回车测试
*/
				if (! ((nextcc = fgetc(stream)) == '\n' && linecount <= 1 ))
					ungetc(nextcc, stream);
				printc = YES;
				continue;
			} else
				ungetc(nextc, stream);  /* 删除注释功能 */

		if (c == '\n') 
			linecount = 0;
#endif	
		if (printc) { 
			putchar(c);
	//		linecount++;
		}

	/* 删除注释功能 */ }
}

int main(void)
{
	
	cleancom(stdin);
	return 0;
}
