#ifndef _LS_HEAD_H
#define _LS_HEAD_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

#define 	ID 		1
#define 	NAME		0
#define	BY_BYTES	1
#define	BY_KB		1024

void print_mode(mode_t st_mode);


#endif
