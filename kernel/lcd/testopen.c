#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define	PIC_NAME "all.jpg"
#define BUF_SIZE 480*272*2

int main(void) {
	int fb_fd, pic_fd;
	char *fb_buf = NULL, save_buf[BUF_SIZE];
	int i;

	if ((fb_fd = open("/dev/my_lcd", O_RDWR)) < 0) {
		perror("open /dev/my_lcd");
		return -1;
	}
	if ((pic_fd = open(PIC_NAME, O_RDONLY)) < 0) {
		perror("open picture");
		close(fb_fd);
		return -1;
	}

	if ((fb_buf = mmap(NULL, BUF_SIZE, PROT_WRITE, MAP_SHARED, fb_fd, 0)) == NULL) {
		perror("mmap");
		close(fb_fd);
		close(pic_fd);
		return -1;
	}
	//
	memcpy(save_buf, fb_buf, BUF_SIZE);
	//
	for (i = 0; i < 60; i++) {
		if (read(pic_fd, fb_buf, BUF_SIZE) < BUF_SIZE)
			perror("read pic to fb_buf");
		sleep(1);
	}

	memcpy(fb_buf, save_buf, BUF_SIZE);
	munmap(fb_buf, BUF_SIZE);
	close(fb_fd);
	close(pic_fd);
	return 0;
}
