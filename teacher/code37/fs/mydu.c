#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <sys/stat.h>
#include <string.h>

#define PATHSIZE 1024

static int
is_looppath(const char *pathstr)
{
	char *filestr;

	filestr = strrchr(pathstr, '/')+1;
	if (strcmp(filestr, ".")==0 || strcmp(filestr, "..")==0) {
		return 1;
	}
	return 0;
}

static int
du(const char *dirpath)
{
	static int ret;
	static struct stat stat_res;
	static char nextpath[PATHSIZE];
	int i;
	int sum;
	glob_t glob_res;

	ret = lstat(dirpath, &stat_res);
	if (ret<0) {
		perror("lstat()");
		return 0;
	}
	if (!S_ISDIR(stat_res.st_mode)) {
		return stat_res.st_blocks;
	}
/***************************/
	snprintf(nextpath, PATHSIZE, "%s/*", dirpath);
	ret = glob(nextpath, GLOB_NOSORT, NULL, &glob_res);
	if (ret!=0) {
		fprintf(stderr, "glob(nextpath) failed.\n");
		return sum;
	}
	snprintf(nextpath, PATHSIZE, "%s/.*", dirpath);
	ret = glob(nextpath, GLOB_NOSORT|GLOB_APPEND, NULL, &glob_res);
	if (ret!=0) {
		fprintf(stderr, "glob(nextpath) failed.\n");
		return sum;
	}
	sum = stat_res.st_blocks;
	for (i=0;i<glob_res.gl_pathc;++i) {
		if (!is_looppath(glob_res.gl_pathv[i])) 
			sum += du(glob_res.gl_pathv[i]);
	}
	globfree(&glob_res);
	return sum;
}

int
main(int argc, char **argv)
{
	if (argc<2) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	printf("%d blocks.\n", du(argv[1]));

	return 0;
}

