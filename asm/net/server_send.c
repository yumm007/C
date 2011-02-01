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
	addr.sin_addr.s_addr = inet_addr("192.168.1.70");


	int i = 0;

	while(1)
	{
		sprintf(buf, "Hello dm9000, %d", i);

		sendto(sock_fd,buf,sizeof(buf),0, (void *)&addr, sizeof(addr));
		sleep(1);
		i++;
	}

	return 0;
}

