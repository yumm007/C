#include "ls.h"

void print_nlink(nlink_t st_nlink, int flag) {
	if (flag == DISABLE)
		return;
	printf("%d ", st_nlink);
}

void print_uid(uid_t st_uid, int flag) {
	if (flag ==DISABLE)
		return;
	if (flag == ID)
		printf("%d ", st_uid);
	else if (flag == NAME)
		printf("%s ", getpwuid(st_uid)->pw_name);
}

void print_gid(gid_t st_gid, int flag) {
	if (flag == DISABLE)
		return;
	if (flag == ID)
		printf("%d ", st_gid);
	else 
		printf("%s ", getgrgid(st_gid)->gr_name);
}

void print_size(off_t st_size, int flag) {
	if (flag == DISABLE)
		return;

	if ( st_size > 0 && st_size / flag <= 0)
		print_size(st_size, flag / 1024);
	else {
		printf("%5ld", st_size / flag);
		if (flag == BY_BYTES)
			printf("B ");
		else if (flag == BY_KB)
			printf("K ");
		else if (flag == BY_M)
			printf("M ");
		else if (flag == BY_G)
			printf("G ");
		/*M, G*/
	}
}

static void __print_time(const time_t *time, int time_f) {
	char tmp[TIMELEN];
	char *time_format;

	switch (time_f) {
		case SHORT_TIME:
			time_format = "%x %X";
			break;
		case LONG_TIME:
			time_format = "%F %T";
			break;
		case FULL_TIME:
			time_format = "%c";
			break;
		default:
			time_format = "%F %T";
			break;
	}
	
	strftime(tmp, TIMELEN, time_format, localtime(time));
	printf("%s ", tmp);
}

void print_time(const struct stat *file_st, int flag, int time_f) {
	if (flag == DISABLE)
		return;
	switch (flag) {
		case BY_MODI:
			__print_time(&file_st->st_mtime, time_f);
			break;
		case BY_ACCE:
			__print_time(&file_st->st_atime, time_f);
			break;
		case BY_CHAN:
			__print_time(&file_st->st_ctime, time_f);
			break;
		default:
			break;
	}
}

