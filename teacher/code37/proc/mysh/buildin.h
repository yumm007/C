#ifndef BUILDIN_H
#define BUILDIN_H

#include "main.h"

typedef int buildin_func_t(struct mysh_cmd_st *);

struct buildin_st {
	char *str;
	buildin_func_t *funcp;
};

extern struct buildin_st buildin[];

int is_buildin(struct mysh_cmd_st *);

#endif

