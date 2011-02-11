#include <stdio.h>

extern char** environ;

int
main()
{
	int i;

	for (i=0;environ[i]!=NULL;++i) {
		puts(environ[i]);
	}
	while(1) pause();
	return 0;
}

