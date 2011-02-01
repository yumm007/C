#include <stdio.h>
#define YES 1
#define NO 0


/*
** 删除注释功能
*/

static void cleancom(FILE *stream) {
	int c, nextc, nextcc, nextccc, linecount = 0;  // 删除注释功能

	while ((c=fgetc(stream)) != EOF) {
		if (c == '/') 		// 删除注释功能
			if ((nextc = fgetc(stream)) == '/') {
				while(fgetc(stream) != '\n')
					;
				if (linecount > 0)
					putchar('\n');
				continue;		/* 删除注释功能 */
			} else	
				ungetc(nextc, stream);
/* 删除注释功能 */
// 删除注释功能
// 删除注释功能

		if (c == '/')
			if ((nextc = fgetc(stream)) == '*') {
				while(nextc = fgetc(stream))	{			/* 删除注释功能_dddddddddddddddddddddddddddddddd */
					if (nextc = '*' && (nextcc = fgetc(stream)) == '/') {
						if (!((nextccc = fgetc(stream)) == '\n' && linecount == 0))
							ungetc(nextccc,stream);
						break;
					} else 
						ungetc(nextcc, stream);
				}
				continue;          /* 删除注释功能_ddddddddddddddddddddddddddddddddddddddddddddd
											ddddddddd */
			} else
				ungetc(nextc, stream);

			putchar(c);
			if (c != '\n')
				linecount++;
			else
				linecount = 0;

	/* 删除注释功能 */ }
}

int main(void)
{
	
	cleancom(stdin);
	return 0;
}
