#ifndef BUILDIN_H
#define BUILDIN_H

#include "main.h"

int is_buildin(struct mysh_cmd_st *);

int do_buildin(int index, struct mysh_cmd_st *);

#endif

