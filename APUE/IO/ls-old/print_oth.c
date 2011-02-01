#include "ls.h"

void print_nlink(nlink_t st_nlink) {
	printf("%d ", st_nlink);
}

void print_uid(uid_t st_uid, int flag) {
	if (flag == ID)
		printf("%d ", st_uid);
	else 
		printf("%s ", getpwuid(st_uid)->pw_name);
}

void print_gid(gid_t st_gid, int flag) {
	if (flag == ID)
		printf("%d ", st_gid);
	else 
		printf("%s ", getgrgid(st_gid)->gr_name);
}

void print_size(off_t st_size, int flag) {

	if ( st_size > 0 && st_size / flag <= 0)
		print_size(st_size, flag / 1024);
	else {
		printf("%5ld", st_size / flag);
		if (flag == BY_BYTES)
			printf("B ");
		else if (flag == BY_KB)
			printf("K ");
		/*M, G*/
	}
}
