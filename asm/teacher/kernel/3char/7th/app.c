#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>



int main(void)
{
	int fd;	
	char buf[50];
	fd = open("/dev/test", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	lseek(fd, 15, SEEK_SET);
	write(fd, "abcdefghigk", 12);
	lseek(fd, -3, SEEK_CUR);
	read(fd, buf, 3);		
	printf("<app>read after lseek:[%s]\n", buf);
	
	return 0;
}
