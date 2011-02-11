#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

#define PATHSTR "/etc"

int
main()
{
	DIR *dp;
	struct dirent *res;

	dp = opendir(PATHSTR);
	if (dp==NULL) {
		perror("opendir()");
		exit(1);
	}

	while ((res=readdir(dp))!=NULL) {
		puts(res->d_name);
	}

	closedir(dp);

	return 0;
}

