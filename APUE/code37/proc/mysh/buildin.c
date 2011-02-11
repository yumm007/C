#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>

#include "buildin.h"

static buildin_func_t func_cd, func_export, func_exit;

struct buildin_st buildin[]={
	{"cd", func_cd},
	{"export", func_export},
	{"exit", func_exit},
	{NULL, NULL},
};

int is_buildin(struct mysh_cmd_st *ptr)
{
	int i;
	for (i=0;buildin[i].str!=NULL;++i) {
		if (strcmp(buildin[i].str, ptr->glob_res.gl_pathv[0])==0) {
			return i;
		}
	}
	return -1;
}

static int func_cd(struct mysh_cmd_st *ptr)
{
	int ret;

	if (ptr->glob_res.gl_pathc>=2) {
		ret = chdir(ptr->glob_res.gl_pathv[1]);
	} else {
		ret = chdir(getenv("HOME"));
	}
	if (ret!=0) {
		perror("chdir");
	}
	return ret;
}

static int func_export(struct mysh_cmd_st *ptr)
{
	return 0;
}

static int func_exit(struct mysh_cmd_st *ptr)
{
	if (ptr->glob_res.gl_pathc>=2) {
		exit(atoi(ptr->glob_res.gl_pathv[1]));
	} else {
		exit(0);
	}
	return 0;
}

