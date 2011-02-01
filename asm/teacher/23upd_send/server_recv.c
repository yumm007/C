#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
	char buf[1024];
	int sock_fd;
	struct sockaddr_in addr;

	sock_fd = socket(AF_INET,SOCK_DGRAM,0);

	bzero(&addr,sizeof(struct sockaddr_in));

	addr.sin_family = AF_INET;
	
	addr.sin_port = htons(1234);

	addr.sin_addr.s_addr = INADDR_ANY;

	bind(sock_fd,(void *)&addr,sizeof(struct sockaddr_in));

	while(1)
	{
		bzero(buf,1024);
		recvfrom(sock_fd,buf,1024,0,NULL,NULL);
		puts(buf);
	}

	return 0;
}

