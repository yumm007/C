#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

#define PAT1 "/etc/*"
#define PAT2 "/etc/.*"

int
main()
{
	int ret, i;
	glob_t glob_res;

	ret = glob(PAT1, GLOB_NOSORT, NULL, &glob_res);
	if (ret!=0) {
		fprintf(stderr, "Failed.\n");
		exit(1);
	}
	ret = glob(PAT2, GLOB_NOSORT|GLOB_APPEND, NULL, &glob_res);
	if (ret!=0) {
		fprintf(stderr, "Failed.\n");
		exit(1);
	}

	for (i=0;i<glob_res.gl_pathc;++i) {
		puts(glob_res.gl_pathv[i]);
	}

	globfree(&glob_res);

	return 0;
}

