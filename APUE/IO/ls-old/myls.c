#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>

#include "ls.h"

#define	BY_BYTES 1
#define	ACCESS_DATE 1
#define	FORMAT_MAX	256

static void print_file_info(struct stat *file_st, char *format) {
	char c;
	int uid_flag = NAME, size_flag = BY_KB;

	if (format == NULL)
		return;

	while ((c = *format++) != '\0')
		switch (c) {
		case 'l':
			print_mode(file_st->st_mode);
			print_nlink(file_st->st_nlink);
			print_uid(file_st->st_uid, uid_flag);
			print_gid(file_st->st_gid, uid_flag);
			print_size(file_st->st_size, size_flag);
#if 0
			print_date(file_st, ACCESS_DATE);
#endif
			break;
		default:
			break;
		}

	return;
}

static int get_format(char *format, char **argv) {
	int i = 0;
	format[0] = '\0';
	while (*++argv != NULL)
		if ((*argv)[0] == '-' && \
			(strlen(format) + strlen(*argv) - 1) < FORMAT_MAX) {
			strcat(format, &(*argv)[1]);
			i++;
		}
	return i;
}

void output_single_file(char *filename, char *format) {
	struct stat file_st;

	if (lstat(filename, &file_st) == -1) {
		fprintf(stderr, "stat %s failed\n", filename);
		return;
	}
	print_file_info(&file_st, format);
	printf("%s\n", filename);

	return;
}

int main(int argc, char *argv[])
{
	char format[FORMAT_MAX], *filename;
	static glob_t dir;
	size_t i;

	if (argc - get_format(format, argv) == 1) {
		if (glob("*", 0, NULL, &dir) != 0)
			return -1;
		for (i = 0; i < dir.gl_pathc; i++)
			output_single_file(dir.gl_pathv[i], format);
	} else {
		while ((filename = *++argv) != NULL) {
			if (filename[0] == '-') 
				continue;
			if (glob(*argv, 0, NULL, &dir) != 0)
				return -1;
			for (i = 0; i < dir.gl_pathc; i++)
				output_single_file(dir.gl_pathv[i], format);
		}
	}

	return 0;
}
