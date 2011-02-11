#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

//#define PAT "/et[cC]/*.co?f"
#define PAT "/etc/*"

int
main()
{
	int ret, i;
	glob_t glob_res;

	ret = glob(PAT, 0, NULL, &glob_res);
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

