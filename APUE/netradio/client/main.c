#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include <custom.h>

#define RCVADDR "0.0.0.0"

struct arg_st args;

static int install_signal(void) {
	struct sigaction sig;
	sigset_t newsig, oldsig;

	sigfillset(&newsig);
	sigprocmask(SIG_BLOCK, &newsig, &oldsig);

	sig.sa_handler = player_cancel;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;

	if (sigaction(SIGINT, &sig, NULL) != 0)
		return -1;

	sigemptyset(&oldsig);
	sigaddset(&oldsig, SIGINT);
	sigprocmask(SIG_UNBLOCK, &oldsig, NULL);

	return 0;
}

int main(int argc, char *argv[])
{
	int out;
	FILE *fp;

	if (change_arg(&args, argc, argv) != 0) 
		exit(1);

	if (install_signal() != 0) {
		fprintf(stderr, "install signal failed, exit.\n");
		exit(1);
	}

	if (change_arg(&args, argc, argv) != 0) 
		exit(1);
	
	if ((init_socket(DEFAULT_MGROUP, DEFAULT_RCVPORT)) != -1) {
		if ((fp = popen(args.player_command, "w")) == NULL) {
			perror("setsockopt()");
			exit(1);
		}
		out = fileno(fp);
		player(out);
		if (pclose(fp) == -1)
			perror("pclose()");
	}
	printf("byebye.\n");
	return 0;
}

