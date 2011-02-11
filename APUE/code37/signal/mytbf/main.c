#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include "mytbf.h"

#define CPS 10
#define BURST 10

#define BUFSIZE 1024

int
main(int argc, char **argv)
{
	int sfd, dfd;
	char buf[BUFSIZE];
	int len, pos, ret, retcode=0;
	MYTBF *context;

	if (argc<2) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	context = tbf_context_new(CPS, BURST);
	if (context==NULL) {
		exit(1);
	}

	sfd = open(argv[1], O_RDONLY);
	if (sfd<0) {
		perror("open(sfd)");
		exit(1);
	}
	dfd = 1;

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
		while (len>0) {
			ret = tbf_write(dfd, buf+pos, len, context);
			if (ret<0) {
				if (errno==EINTR) {
					continue;
				}
				perror("write()");
				retcode=1;
				exit(1);
			}
			len -= ret;
			pos += ret;
		}
	}

	close(sfd);

	tbf_context_delete(context);

	return retcode;
}

