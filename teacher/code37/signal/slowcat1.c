#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define CPS 10
#define BURST 10

#define BUFSIZE CPS

static sig_atomic_t token=0;

static void
alrm_handler(int s)
{
	alarm(1);
	token++;
	if (token>BURST) {
		token=BURST;
	}
}

int
main(int argc, char **argv)
{
	int sfd, dfd;
	char buf[BUFSIZE];
	int len, pos, ret, retcode=0;

	if (argc<2) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	sfd = open(argv[1], O_RDONLY);
	if (sfd<0) {
		perror("open(sfd)");
		exit(1);
	}
	dfd = 1;

	signal(SIGALRM, alrm_handler);
	alarm(1);

	len=0;
	while (1) {
		while (token==0) pause();
		token--;
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
			ret = write(dfd, buf+pos, len);
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

	return retcode;
}

