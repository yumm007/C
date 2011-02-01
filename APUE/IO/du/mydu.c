#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/types.h>
#include <dirent.h>

#define	MAXDIR	4096

static blkcnt_t mydu(char *filename) {
	static struct stat file_st;
	static struct dirent *next_file;
	DIR *cur_dir;
	size_t dir_len;
	blkcnt_t sum;
	
	/* stat file error*/
	if (lstat(filename, &file_st) == -1) {
		perror(filename);
		return 0;
	}

	/* not directory*/
	if (!S_ISDIR(file_st.st_mode))
		return file_st.st_blocks;

	/* is directory*/
	if ((cur_dir = opendir(filename)) == NULL) {
		perror(filename);
		return 0;
	}

	sum = file_st.st_blocks;
	dir_len = strlen(filename);

	while ((next_file = readdir(cur_dir)) != NULL) {
		if (strcmp(next_file->d_name, ".") == 0 ||
			strcmp(next_file->d_name, "..") == 0)
			continue;
		if (dir_len + 1 + strlen(next_file->d_name) >= MAXDIR) 
			continue;
		
		strcat(filename, "/");
		strcat(filename, next_file->d_name);
		sum += mydu(filename);
		filename[dir_len] = '\0';
	}

	closedir(cur_dir);

	return sum;
}

static void get_dir(char *dir, const char *argv) {
	strncpy(dir, argv, MAXDIR);
	if (dir[strlen(dir) - 1] == '/') 
		dir[strlen(dir) - 1] = '\0';
	return;
}

int main(int argc, char *argv[])
{
	char dir[MAXDIR];

	if (argc == 1) {
		strcpy(dir, ".");
		printf("%ld\t%s\n",mydu(dir) / 2, dir);
	} else { 
		while (*++argv != NULL) {
			get_dir(dir, *argv);
			printf("%ld\t%s\n",mydu(dir) / 2, dir);
		}
	}

	return 0;
}
