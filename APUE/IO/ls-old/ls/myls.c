#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>

#include "ls.h"

#define	FORMAT_MAX	256
#define	MAXDIR	4096

static void print_file_info(const struct stat *file_st, const struct print_f *format) {

	print_mode(file_st->st_mode, format->mode_flag);
	print_nlink(file_st->st_nlink, format->link_flag);
	print_uid(file_st->st_uid, format->id_name_flag);
	print_gid(file_st->st_gid, format->id_name_flag);
	print_size(file_st->st_size, format->size_flag);
	print_time(file_st, format->date_flag, format->time_form);

	return;
}

static int get_format(struct print_f *format, char **argv) {
	int i = 0, j;
	char c;

	/*default format value*/
	format->time_form = LONG_TIME;
//	format->wd_flag = ENABLE;

	/*get format from argv*/
	while (*++argv != NULL)
		if ((*argv)[0] == '-') {
			j = 1;
			while ( (c = (*argv)[j++]) != '\0')
				switch (c) {
				case 'l':
					format->mode_flag = ENABLE;
					format->link_flag = ENABLE;
					format->date_flag = BY_MODI;
					format->size_flag = BY_BYTES;
					format->id_name_flag = NAME;
					format->newl_flag	= ENABLE;
					break;
				case 'c':
					format->date_flag = BY_CHAN;
					break;
				case 'u':
					format->date_flag = BY_ACCE;
					break;
				case 'k':
					format->size_flag = BY_KB;
					break;
				case 'n':
					format->mode_flag = ENABLE;
					format->link_flag = ENABLE;
					format->date_flag = BY_MODI;
					format->size_flag = BY_BYTES;
					format->id_name_flag = ID;
					format->newl_flag	= ENABLE;
					break;
				case 'R': /*递归目录*/
					format->dire_flag = ENABLE;
					break;
				case 'a':
					format->hide_flag = ENABLE;
					format->dot_flag	= ENABLE;
					break;
				case 'A':
					format->hide_flag = ENABLE;
					format->dot_flag	= DISABLE;
					break;
				case 'h':
					format->size_flag = BY_G;
					break;
				case 'd': /*显示目录信息*/
					format->dire_flag = DISABLE;
					format->wd_flag = ENABLE;
					format->dot_flag	= ENABLE;
					break;
				default:
					 break;
				}
			i++;
		}
	return i;
}

static int is_dot_file(const char *filename) {
	if (strrchr(filename,'.') == filename +strlen(filename) - 1) 
		return 1;
	return 0;
}

static int is_dir(const char *filename) {
	static struct stat file_st;
	if (lstat(filename, &file_st) == -1) {
		perror(filename);
		return 0;
	}
	return S_ISDIR(file_st.st_mode);
}

static void rec_dir(const glob_t *dir, const struct print_f *format) {
	int i;
	if (format->dire_flag) { /* 如果递归显示目录，则按照次序显示目录内容*/
		/* list doc file first  */
		for (i = 0; i < dir->gl_pathc; i++) {
			if (is_dot_file(dir->gl_pathv[i]))
				output_single_file(dir->gl_pathv[i], format);
		}
		/* list file first  */
		for (i = 0; i < dir->gl_pathc; i++) {
			if (is_dir(dir->gl_pathv[i]))
				continue;
			output_single_file(dir->gl_pathv[i], format);
		}
		/*then directory*/
		for (i = 0; i < dir->gl_pathc; i++) {
			if (is_dir(dir->gl_pathv[i]) && !is_dot_file(dir->gl_pathv[i]))
				output_single_file(dir->gl_pathv[i], format);
		}
	} else {
		for (i = 0; i < dir->gl_pathc; i++)
			output_single_file(dir->gl_pathv[i], format);
	}

	return;
}

void static print_single_record(const struct stat *file_st, const struct\
	print_f *format, const char *filename) {

	char *p;

	if (is_dot_file(filename) && (format->dot_flag) == DISABLE) 
		return;
	
	print_file_info(file_st, format);
	if ((p = strrchr(filename, '/')) != NULL && \
		p - filename + 1 != strlen(filename))
		printf("%s", p + 1);
	else
		printf("%s", filename);
	printf("%c", format->newl_flag ? '\n': '\t');

	return;
}

void output_single_file(char *filename, const struct print_f *format) {
	static struct stat file_st;
	static char file_path[MAXDIR];
	glob_t dir;
	int glob_mode = 0;

	if (lstat(filename, &file_st) == -1) {
		perror(filename);
		return;
	}

	/*如果这不是一个目录，或者递归目录没开，则显示这条记录*/
	if (!S_ISDIR(file_st.st_mode) || format->dire_flag == DISABLE)
		print_single_record(&file_st, format, filename);

	/*if -R, 但要忽略 . 和 .. 两个目录*/
	if (format->dire_flag && S_ISDIR(file_st.st_mode)) {
		if (is_dot_file(filename)) {
			print_single_record(&file_st, format, filename);
			return;
		}

		/* 获取下级目录项*/
		if (format->hide_flag) {
			sprintf(file_path, "%s/.*", filename);
			if (glob(file_path, 0, NULL, &dir) != 0)
				return;
			glob_mode = GLOB_APPEND;
		}
		sprintf(file_path, "%s/*", filename);
		if (glob(file_path, glob_mode, NULL, &dir) != 0)
			return;

		printf("\n%s:\n", filename);
		rec_dir(&dir, format);
	}

	return;
}

static char *extend_dir(char *oldfile, const struct print_f *format, int isfirst) {
	static char file_path[MAXDIR];
	char *p;

	if (!is_dir(oldfile))
		return oldfile;
	
	if (is_dir(oldfile) && format->wd_flag)
		return oldfile;

	strncpy(file_path, oldfile, MAXDIR - 3);
	if ((p = strrchr(file_path, '/')) != NULL && \
			*p == file_path[strlen(file_path) -1])
		*p = '\0';
	strcat(file_path, "/");
	if (format->hide_flag && isfirst) 
		strcat(file_path, ".");
	strcat(file_path, "*");

	return file_path;
}

int main(int argc, char *argv[])
{
	char *filename;
	struct print_f format = {0,0,0,0,0,0,0,0,0,0,0};
	static glob_t dir;
	char *argv_temp[3], **my_argv;
	int glob_mode = 0;

	argv_temp[0] = argv[0];
	argv_temp[1] = "*";
	argv_temp[2] = NULL;

	/* 没有指定任何文件名 */
	if (argc - get_format(&format, argv) == 1) 
		my_argv = argv_temp;
	else
		my_argv = argv;
	
	if (format.hide_flag)
		argv_temp[1] = ".";

	while ((filename = *++my_argv) != NULL) {
		if (filename[0] == '-')  /* 忽略参数*/
			continue;
		if (format.hide_flag) {
			if (glob(extend_dir(*my_argv, &format, 1), 0, NULL, &dir) != 0)
				continue;
			glob_mode = GLOB_APPEND;
		}
		if (glob(extend_dir(*my_argv, &format, 0),glob_mode, NULL, &dir) != 0)
			continue;
		rec_dir(&dir, &format);
	}

	if (format.newl_flag == DISABLE) /* 末行需要打印回车*/
		printf("\n");

	return 0;
}
