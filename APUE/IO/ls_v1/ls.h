#ifndef _LS_HEAD_H
#define _LS_HEAD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#define	CACHE		0

#define	MODE_LEN		16
#define	INO_LEN		10
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

#define	SORT_BY_NAME	1

struct print_f {
	int mode_flag;
	int ino_flag;
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
	int sort_flag;
};

struct outline_t {
	char mode[MODE_LEN + 1];
	char ino[INO_LEN + 1];
	char nlink[NLINK_LEN + 1];
	char uid[UID_LEN + 1];
	char gid[GID_LEN + 1];
	char size[SIZE_LEN + 1];
	char time[TIME_LEN + 1];
	char name[NAME_LEN + 1];
};

int print_mode(mode_t st_mode, int flag, char *buf);
int print_ino(ino_t st_ino, int flag, char *buf);
int print_nlink(nlink_t st_nlink, int flag, char *buf);
int print_uid(uid_t st_uid, int flag, char *buf);
int print_gid(gid_t st_gid, int flag, char *buf);
int print_size(off_t st_size, int flag, char *buf);
int print_time(const struct stat *file_st, int flag, int time_f, char *buf);
void output_single_file(char *filename, const struct print_f *format);
void print_single_record(const struct stat *file_st, const struct\
	print_f *format, const char *filename, int flag);
void print_buf(struct outline_t *line);
void print_file_info(const struct stat *file_st, const struct print_f *format, struct outline_t *outl);
void flush_line(struct outline_t *outl, int num, const struct print_f *format);
int extend_line_buf(struct outline_t **buf,int * bufsize);
int is_dot_file(const char *filename);
int is_dir(const char *filename);
int print_color_name(const char * mode, const char *filename);
#endif
