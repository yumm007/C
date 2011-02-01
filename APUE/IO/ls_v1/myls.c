#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>

#include "ls.h"

#define	FORMAT_MAX	256
#define	MAXDIR	4096

static int get_format(struct print_f *format, char **argv, int *err_opt) {
	int i = 0, j, c;

	/*default format value*/
	format->time_form = LONG_TIME;
	format->sort_flag = SORT_BY_NAME;

	/*get format from argv*/
	while (*++argv != NULL)
		if ((*argv)[0] == '-') {
			j = 1;
			while ( (c = (*argv)[j++]) != '\0')
				switch (c) {
				case 'S':
					if (*++argv == NULL)
						return -1;
					if (strcmp("name", *argv) == 0)
						format->sort_flag = SORT_BY_NAME;
					(*argv)[0] = '\0';
					j = 0;
					i++;
					break;
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
					format->newl_flag	= ENABLE;
					break;
				case 'u':
					format->date_flag = BY_ACCE;
					format->newl_flag	= ENABLE;
					break;
				case 'k':
					format->size_flag = BY_KB;
					format->newl_flag	= ENABLE;
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
					format->newl_flag	= ENABLE;
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
					format->newl_flag	= ENABLE;
					break;
				case 'i':
					format->ino_flag = ENABLE;
					format->newl_flag	= ENABLE;
					break;
				case '0':
					return -1;
					break;
				case 'd': /*显示目录信息*/
					format->dire_flag = DISABLE;
					format->wd_flag = ENABLE;
					format->dot_flag	= ENABLE;
					format->newl_flag	= ENABLE;
					break;
				default:
					*err_opt = c;
					return -1;
					break;
				}
			i++;
		}
	return i;
}

int is_dot_file(const char *filename) {
	return (strrchr(filename,'.') == filename +strlen(filename) - 1);
}

int is_dir(const char *filename) {
	static struct stat file_st;
	if (lstat(filename, &file_st) == -1){
		perror(filename);
		return 0;
	}
	return S_ISDIR(file_st.st_mode);
}

static void rec_dir(const glob_t *dir, const struct print_f *format) {
	int i;
	struct print_f tmp_format = *format;
	tmp_format.dire_flag = DISABLE;

	/* list file first  */
	for (i = 0; i < dir->gl_pathc; i++) 
		output_single_file(dir->gl_pathv[i], &tmp_format);
	print_single_record(NULL,format,NULL,FLUSH);

	if (format->dire_flag)  /* 如果递归显示目录，则按照次序显示目录内容*/
		for (i = 0; i < dir->gl_pathc; i++) 
			if (is_dir(dir->gl_pathv[i]))
				output_single_file(dir->gl_pathv[i], format);

	return;
}

static char *read_link(const char *filename) {
	static char file_name[NAME_LEN + 1], link_name[NAME_LEN + 1];
	static size_t link_len;
	
	strncpy(file_name, filename, NAME_LEN - strlen(" -> "));
	strcat(file_name, " -> ");
	if ((link_len = readlink(filename, link_name, NAME_LEN - strlen(file_name))) == -1)
		return NULL;
	link_name[link_len] = '\0';
	strcat(file_name, link_name);
	
	return file_name;
}

static int errfunc(const char *epath, int eerrno) {
	switch (eerrno) {
	case GLOB_NOSPACE:
		fprintf(stderr, "running out of memory.\n");
		break;
	case GLOB_NOMATCH:
		fprintf(stderr, "no found item.\n");
		break;
	case GLOB_ABORTED:
		fprintf(stderr, "read error.\n");
		break;
	default:
		perror(epath);
		break;
	}

	return 0;
}

void output_single_file(char *filename, const struct print_f *format) {
	static struct stat file_st;
	static char file_path[MAXDIR];
	glob_t dir;
	int glob_mode = GLOB_NOSORT;

	if (lstat(filename, &file_st) == -1) {
		perror(filename);
		return;
	}
	if (S_ISLNK(file_st.st_mode))
		if ((filename = read_link(filename)) == NULL) {
			perror(filename);
			return;
		}

	if (format->dire_flag == DISABLE)
		print_single_record(&file_st,format,filename,0);
	/*if -R, 但要忽略 . 和 .. 两个目录*/
	else if (S_ISDIR(file_st.st_mode)) {
		if (is_dot_file(filename))
			return;
		/* 获取下级目录项*/
		if (format->hide_flag) {
			sprintf(file_path, "%s/.*", filename);
			if (glob(file_path, 0, errfunc, &dir) != 0)
				return;
			glob_mode |= GLOB_APPEND;
		}
		sprintf(file_path, "%s/*", filename);
		if (glob(file_path, glob_mode, errfunc, &dir) != 0) {
			if (glob_mode == (GLOB_NOSORT | GLOB_APPEND))
				globfree(&dir);
			return;
		}

		printf("\n%s:\n", filename);
		rec_dir(&dir, format);
		globfree(&dir);
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

static void print_help(const char *help_file, int err_opt) {
	FILE *fp;
	char *line = NULL;
	size_t len = 0;

	if (err_opt != -1)
		fprintf(stderr, "\nUnknown option: %c\n\n", err_opt);
	if ((fp = fopen(help_file, "r")) == NULL) {
		perror(help_file);
		return;
	}
	while (getline(&line, &len, fp) > 0)
		fprintf(stderr, "%s", line);
	fclose(fp);
}

int main(int argc, char *argv[])
{
	char *filename;
	struct print_f format = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	static glob_t dir;
	char *argv_temp[3], **my_argv;
	int glob_mode = GLOB_NOSORT, ret, err_opt = -1;

	argv_temp[0] = argv[0];
	argv_temp[1] = ".";
	argv_temp[2] = NULL;

	if ((ret = get_format(&format, argv, &err_opt)) == -1) {
		print_help("help", err_opt);
		return -1;
	}
	/* 没有指定任何文件名 */
	if (argc - ret == 1) 
		my_argv = argv_temp;
	else
		my_argv = argv;
	
	if (format.hide_flag)
		argv_temp[1] = ".";

	while ((filename = *++my_argv) != NULL) {
		/* 忽略参数*/
		if (strlen(filename) == 0 || filename[0] == '-') 
			continue;
		if (format.hide_flag) {
			if (glob(extend_dir(*my_argv, &format, 1), 0, errfunc, &dir) != 0)
				continue;
			glob_mode |= GLOB_APPEND;
		}
		if (glob(extend_dir(*my_argv, &format, 0),glob_mode, errfunc, &dir) != 0) {
			if (glob_mode == (GLOB_NOSORT|GLOB_APPEND))
				globfree(&dir);
			continue;
		}
		rec_dir(&dir, &format);
		globfree(&dir);
	}

	return 0;
}
