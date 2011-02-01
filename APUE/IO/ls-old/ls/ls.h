#ifndef _LS_HEAD_H
#define _LS_HEAD_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include	<time.h>

#define	ENABLE	1
#define	DISABLE	0
#define 	NAME		1
#define 	ID 		2
#define	BY_BYTES	1
#define	BY_KB		1<<10
#define	BY_M		BY_KB<<10
#define	BY_G		BY_M<<10

#define BY_MODI	1
#define BY_ACCE	2
#define BY_CHAN	3

#define	TIMELEN		256
#define	SHORT_TIME	1
#define	LONG_TIME	2
#define	FULL_TIME	3

struct print_f {
	int mode_flag;
	int link_flag;
	int id_name_flag;
	int size_flag;
	int date_flag;
	int time_form;
	int dire_flag; /*递归目录*/
	int newl_flag;
	int hide_flag; /* 显示隐藏文件*/
	int wd_flag;	/* 显示目录*/
	int dot_flag;	/*显示 . 和 .. */
};

void print_mode(mode_t st_mode, int flag);
void print_nlink(nlink_t st_nlink, int flag);
void print_uid(uid_t st_uid, int flag);
void print_gid(gid_t st_gid, int flag);
void print_size(off_t st_size, int flag);
void print_time(const struct stat *file_st, int flag, int time_f);
void output_single_file(char *filename, const struct print_f *format);

#endif
