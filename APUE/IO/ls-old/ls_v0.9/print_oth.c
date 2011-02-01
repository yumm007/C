#include "ls.h"

int print_ino(ino_t st_ino, int flag, char *buf) {
	int n;

	switch (flag) {
	case DISABLE:
		n = 0;
		break;
	case	ENABLE:
		n = snprintf(buf, INO_LEN, "%lu", st_ino);
		break;
	default:
		n = 0;
		break;
	}
	buf[n] = '\0';

	return n;
}

int print_nlink(nlink_t st_nlink, int flag, char *buf) {
	int n;

	switch (flag) {
	case DISABLE:
		n = 0;
		break;
	case	ENABLE:
		n = snprintf(buf, NLINK_LEN, "%d", st_nlink);
		break;
	default:
		n = 0;
		break;
	}
	buf[n] = '\0';

	return n;
}

int print_uid(uid_t st_uid, int flag, char *buf) {
	int n;
	struct passwd *pw;

	switch (flag) {
	case	DISABLE:
		n = 0;
		break;
	case ID:
		n = snprintf(buf, UID_LEN, "%u", st_uid);
		break;
	case NAME:
		if ((pw = getpwuid(st_uid)) == NULL)
			n = snprintf(buf, UID_LEN, "%u", st_uid);
		else
			n = snprintf(buf,UID_LEN,"%s", pw->pw_name);
		break;
	default:
		n = 0;
		break;
	}
	buf[n] = '\0';
	
	return n;
}

int print_gid(gid_t st_gid, int flag, char *buf) {
	int n;
	struct group *gp;

	switch (flag) {
	case	DISABLE:
		n = 0;
		break;
	case	ID:
		n = snprintf(buf, GID_LEN, "%d", st_gid);
		break;
	case	NAME:
		if ((gp = getgrgid(st_gid)) == NULL)
			n = snprintf(buf, GID_LEN, "%d", st_gid);
		else
			n = snprintf(buf, GID_LEN, "%s", gp->gr_name);
		break;
	default:
		n = 0;
		break;
	}
	buf[n] = '\0';

	return n;
}

int print_size(off_t st_size, int flag, char *buf) {
	int n;
	if (flag == DISABLE) {
		buf[0] = '\0';
		return 0;
	}

	if ( st_size > 0 && st_size / flag <= 0)
		return print_size(st_size, flag / 1024, buf);
	else {
		if (flag == BY_BYTES)
			n = snprintf(buf, SIZE_LEN, "%ld",st_size / flag);
		else
			n = snprintf(buf, SIZE_LEN, "%.1f",(double)st_size / flag);
		buf[n] = '\0';
		if (st_size == 0 || flag == BY_BYTES || n == SIZE_LEN)
			;//strcat(buf, "");
		else if (flag == BY_KB)
			strcat(buf, "K");
		else if (flag == BY_M)
			strcat(buf, "M");
		else if (flag == BY_G)
			strcat(buf, "G");
		return strlen(buf);
	}
}

static int __print_time(const time_t *time, int time_f, char *buf) {
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
	
	return strftime(buf, TIME_LEN, time_format, localtime(time));
}

int print_time(const struct stat *file_st, int flag, int time_f, char *buf) {
	int n;
	if (flag == DISABLE) {
		buf[0] = '\0';
		return 0;
	}
	switch (flag) {
		case BY_MODI:
			n =  __print_time(&file_st->st_mtime, time_f, buf);
			break;
		case BY_ACCE:
			n =  __print_time(&file_st->st_atime, time_f, buf);
			break;
		case BY_CHAN:
			n = __print_time(&file_st->st_ctime, time_f, buf);
			break;
		default:
			break;
	}
	
	return n;
}

