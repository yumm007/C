#ifndef _LS_HEAD_H
#define _LS_HEAD_H

#include <stdio.h>
#include <string.h>
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
#define	MAXLINE	1024
#define	FLUSH		1

#define	MODE_LEN		16
#define	NLINK_LEN	10
#define	UID_LEN		20
#define	GID_LEN		20
#define	SIZE_LEN		20
#define	TIME_LEN		40
#define	NAME_LEN		256

#define BY_MODI	1
#define BY_ACCE	2
#define BY_CHAN	3

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

struct outline_t {
	char mode[MODE_LEN];
	char nlink[NLINK_LEN];
	char uid[UID_LEN];
	char gid[GID_LEN];
	char size[SIZE_LEN];
	char time[TIME_LEN];
	char name[NAME_LEN];
};

void print_mode(mode_t st_mode, int flag, char *buf);
void print_nlink(nlink_t st_nlink, int flag, char *buf);
void print_uid(uid_t st_uid, int flag, char *buf);
void print_gid(gid_t st_gid, int flag, char *buf);
void print_size(off_t st_size, int flag, char *buf);
void print_time(const struct stat *file_st, int flag, int time_f, char *buf);
void output_single_file(char *filename, const struct print_f *format);

#endif
