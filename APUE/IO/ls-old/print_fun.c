#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define	BY_BYTES 1
#define	ACCESS_DATE 1

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

	format[1] = '\0';
	printf("%s", format);
}









