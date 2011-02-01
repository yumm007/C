#include <stdio.h>
#define YES 1
#define NO 0



static void cleancom(FILE *stream) {
	int c, nextc, nextcc, nextccc, linecount = 0;  

	while ((c=fgetc(stream)) != EOF) {
		if (c == '/') 		
			if ((nextc = fgetc(stream)) == '/') {
				while(fgetc(stream) != '\n')
					;
				if (linecount > 0)
					putchar('\n');
				continue;		
			} else	
				ungetc(nextc, stream);

		if (c == '/')
			if ((nextc = fgetc(stream)) == '*') {
				while(nextc = fgetc(stream))	{			
					if (nextc = '*' && (nextcc = fgetc(stream)) == '/') {
						if (!((nextccc = fgetc(stream)) == '\n' && linecount == 0))
							ungetc(nextccc,stream);
						break;
					} else 
						ungetc(nextcc, stream);
				}
				continue;          
			} else
				ungetc(nextc, stream);

			putchar(c);
			if (c != '\n')
				linecount++;
			else
				linecount = 0;

	 }
}

int main(void)
{
	
	cleancom(stdin);
	return 0;
}
