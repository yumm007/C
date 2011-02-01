#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	fd = open("biffile", O_WRONLY | O_CREAT| O_TRUNC);
	lseek(fd, 1ll*1024ll*1024ll*1024ll, SEEK_SET);
	write(fd, "aaaa", 5);	
	close(fd);

	return 0;
}
