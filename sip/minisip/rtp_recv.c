#include <stdio.h>
#include <string.h>
#include "minisip.h"

int main(int argc, char **argv) {
	int sd;
	struct sockaddr_in addr;
	char tmp[1024];
	ssize_t recv_number;
	socklen_t addrlen = sizeof(addr);

	if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket:");
		return -1;
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(LOCAL_RTP_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sd, (void *)&addr, sizeof(addr)) == -1) {
		close(sd);
		perror("bind:");
		return -1;
	}

	while (1) {
		memset(tmp, 0, sizeof(tmp));
		recv_number = recvfrom(sd, tmp, 1024, 0, (void *)&addr, (void *)&addrlen);
		//输出RTP数据部分
		if (recv_number > 12)
			fwrite(tmp + 12, 1, recv_number, stdout);
			sendto(sd, tmp, 1024, 0, (void *)&addr, addrlen);
	}

	close(sd);

	return 0;
}
