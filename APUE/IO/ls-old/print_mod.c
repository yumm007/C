#include "ls.h"

void print_mode(mode_t st_mode) {
	char format[17];

	/*file type*/
	switch (st_mode & S_IFMT) {
		case S_IFSOCK:
			format[0] = 's';
			break;
		case S_IFLNK:
			format[0] = 'l';
			break;
		case S_IFREG:
			format[0] = '-';
			break;
		case S_IFBLK:
			format[0] = 'b';
			break;
		case S_IFDIR:
			format[0] = 'd';
			break;
		case S_IFCHR:
			format[0] = 'c';
			break;
		case S_IFIFO:
			format[0] = 'f';
			break;
		case S_ISUID:
			format[3] = 's';
			break;
		case S_ISGID:
			format[6] = 's';
			break;
		case S_ISVTX:
			format[9] = 't';
			break;
		default:
			format[0] = '?';
		break;
	}
	
	/* file permission*/
	format[1] = format[2] = format[3] = '-';
	format[4] = format[5] = format[6] = '-';
	format[7] = format[8] = format[9] = '-';

		/*User permission*/
	if (((st_mode & S_IRWXU) & S_IRUSR) == S_IRUSR)
		format[1] = 'r';
	if (((st_mode & S_IRWXU) & S_IWUSR) == S_IWUSR)
		format[2] = 'w';
	if ((st_mode & S_ISUID) == S_ISUID \
		 && ((st_mode & S_IRWXU) & S_IXUSR) == S_IXUSR)
			format[3] = 's';
	else if ((st_mode & S_ISUID) == S_ISUID \
		 && ((st_mode & S_IRWXU) & S_IXUSR) != S_IXUSR)
			format[3] = 'S';
	else if ((st_mode & S_ISUID) != S_ISUID \
		 && ((st_mode & S_IRWXU) & S_IXUSR) == S_IXUSR)
			format[3] = 'x';
	
		/*Group permission*/
	if (((st_mode & S_IRWXG) & S_IRGRP) == S_IRGRP)
		format[4] = 'r';
	if (((st_mode & S_IRWXG) & S_IWGRP) == S_IWGRP)
		format[5] = 'w';
	if ((st_mode & S_ISGID) == S_ISGID \
		 && ((st_mode & S_IRWXG) & S_IXGRP) == S_IXGRP)
			format[6] = 's';
	else if ((st_mode & S_ISGID) == S_ISGID \
		 && ((st_mode & S_IRWXG) & S_IXGRP) != S_IXGRP)
			format[6] = 'S';
	else if ((st_mode & S_ISGID) != S_ISGID \
		 && ((st_mode & S_IRWXG) & S_IXGRP) == S_IXGRP)
			format[6] = 'x';

		/*Other permission*/
	if (((st_mode & S_IRWXO) & S_IROTH) == S_IROTH)
		format[7] = 'r';
	if (((st_mode & S_IRWXO) & S_IWOTH) == S_IWOTH)
		format[8] = 'w';
	if ((st_mode & S_ISVTX) == S_ISVTX \
		 && ((st_mode & S_IRWXO) & S_IXOTH) == S_IXOTH)
			format[9] = 't';
	else if ((st_mode & S_ISVTX) == S_ISVTX \
		 && ((st_mode & S_IRWXO) & S_IXOTH) != S_IXOTH)
			format[9] = 'T';
	else if ((st_mode & S_ISVTX) != S_ISVTX \
		 && ((st_mode & S_IRWXO) & S_IXOTH) == S_IXOTH)
			format[9] = 'x';

	format[10] = '\0';
	printf("%s ", format);
}

