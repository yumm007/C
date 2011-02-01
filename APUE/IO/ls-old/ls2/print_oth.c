#include "ls.h"

void print_nlink(nlink_t st_nlink, int flag, char *buf) {
	int n = 0;
	if (flag == DISABLE) {
		buf[0] = '\0';
		return;
	}
	n = snprintf(buf, NLINK_LEN, "%d", st_nlink);
	buf[n] = '\0';
}

void print_uid(uid_t st_uid, int flag, char *buf) {
	int n = 0;
	struct passwd *pw;
	if (flag ==DISABLE) {
		buf[0] = '\0';
		return;
	}

	if (flag == ID)
		n = snprintf(buf, UID_LEN, "%ld", st_uid);
	else if (flag == NAME) {
		if ((pw = getpwuid(st_uid)) == NULL)
			n = snprintf(buf, UID_LEN, "%ld", st_uid);
		else
			n = snprintf(buf,UID_LEN,"%s", pw->pw_name);
	}
	buf[n] = '\0';
}

void print_gid(gid_t st_gid, int flag, char *buf) {
	int n = 0;
	struct group *gp;

	if (flag == DISABLE){
		buf[0] = '\0';
		return;
	}
	if (flag == ID)
		n = snprintf(buf, GID_LEN, "%d", st_gid);
	else if (flag == NAME) {
		if ((gp = getgrgid(st_gid)) == NULL)
			n = snprintf(buf, GID_LEN, "%d", st_gid);
		else
			n = snprintf(buf, GID_LEN, "%s", gp->gr_name);
	}
	buf[n] = '\0';
}

void print_size(off_t st_size, int flag, char *buf) {
	int n = 0;
	if (flag == DISABLE) {
		buf[0] = '\0';
		return;
	}

	if ( st_size > 0 && st_size / flag <= 0)
		print_size(st_size, flag / 1024, buf);
	else {
		if (flag == BY_BYTES)
			n = snprintf(buf, SIZE_LEN, "%ld",st_size / flag);
		else
			n = snprintf(buf, SIZE_LEN, "%.1f",(double)st_size / flag);
		buf[n] = '\0';
		if (st_size == 0 || flag == BY_BYTES)
			;//strcat(buf, "");
		else if (flag == BY_KB)
			strcat(buf, "K");
		else if (flag == BY_M)
			strcat(buf, "M");
		else if (flag == BY_G)
			strcat(buf, "G");
		/*M, G*/
	}
}

static void __print_time(const time_t *time, int time_f, char *buf) {
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
	
	strftime(buf, TIME_LEN, time_format, localtime(time));
}

void print_time(const struct stat *file_st, int flag, int time_f, char *buf) {
	if (flag == DISABLE) {
		buf[0] = '\0';
		return;
	}
	switch (flag) {
		case BY_MODI:
			__print_time(&file_st->st_mtime, time_f, buf);
			break;
		case BY_ACCE:
			__print_time(&file_st->st_atime, time_f, buf);
			break;
		case BY_CHAN:
			__print_time(&file_st->st_ctime, time_f, buf);
			break;
		default:
			break;
	}
}

