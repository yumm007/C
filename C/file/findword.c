#include <stdio.h>
#define MAXBUF 1024

static int count;

static int notchar(const char c) {
	if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' )
		return 0;
	return 1;
}

static void findword(const char *line, const char *dest) {
	int c, i, j,p;
	i = j = p = 0;
	
	while ( (c = line[p++]) != '\0'){
		if (c != dest[0])
			continue;
		for (i = p, j = 1; line[i] != '\0' && dest[j] != '\0' && line[i] == dest[j]; i++, j++)
			;
		if ( (dest[j] == '\0') && notchar(line[i]) && ( p == 1 || notchar(line[p-2])) ) 
			count++;
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;
	char templine[MAXBUF];

	count = 0;

	if (argc != 3) {
		fprintf(stderr,"Usage: %s filepath searchword\n",argv[0]);
		return 1;
	}

	if ((fp = fopen(argv[1],"r")) == NULL) {
		perror(argv[1]);
		return 1;
	}

	while (fgets(templine, MAXBUF, fp) != NULL)
		findword(templine, argv[2]);

	printf("%d\n",count);
	
	fclose(fp);
	return 0;
}
