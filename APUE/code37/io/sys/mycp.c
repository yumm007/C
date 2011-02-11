#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFSIZE 64

int
main(int argc, char **argv)
{
	int sfd, dfd;
	char buf[BUFSIZE];
	int len, pos, ret, retcode=0;

	if (argc<3) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	sfd = open(argv[1], O_RDONLY);
	if (sfd<0) {
		perror("open(sfd)");
		exit(1);
	}
	dfd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0600);
	if (dfd<0) {
		perror("open(dfd)");
		close(sfd);
		exit(1);
	}

	len=0;
	while (1) {
		if (len==0) {
			len = read(sfd, buf, BUFSIZE);
			if (len<0) {
				if (errno==EINTR) {
					continue;
				}
				perror("read()");
				retcode=1;
				break;
			}
			if (len==0) {
				break;
			}
			pos=0;
		}
		if (len>0) {
			ret = write(dfd, buf+pos, len);
			if (ret<0) {
				if (errno==EINTR) {
					continue;
				}
				perror("write()");
				retcode=1;
				break;
			}
			len -= ret;
			pos += ret;
		}
	}

	close(sfd);
	close(dfd);

	return retcode;
}

