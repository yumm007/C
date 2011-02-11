#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>

#define FNAME "/tmp/daemon.out"

static void
daemon_exit(int s)
{
	syslog(LOG_INFO, "Signal %d recieved, exit now...", s);
	exit(0);
}

static int
daemonize(void)
{
	pid_t pid;
	int fd;

	fflush(NULL);
	pid = fork();
	if (pid<0) {
		return -1;
	}
	if (pid!=0) {
		exit(0);
	}
	fd = open("/dev/null", O_RDWR);
	if (fd<0) {
		syslog(LOG_WARNING, "open(): %s", strerror(errno));
		syslog(LOG_WARNING, "Will nor redirect stdin, stdout, stderr.");
		close(0);
		close(1);
		close(2);
	} else {
		dup2(fd, 0);
		dup2(fd, 1);
		dup2(fd, 2);
		if (fd>2) close(fd);
	}
	chdir("/");
	umask(0);
	setsid();
	return 0;
}

int
main()
{
	int i;
	FILE *fp;
	struct sigaction sa;

	sa.sa_handler = daemon_exit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGTERM);
	sigaddset(&sa.sa_mask, SIGINT);
	sigaddset(&sa.sa_mask, SIGQUIT);
	sa.sa_flags = SA_RESTART;

	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);


	openlog("testdaemon", LOG_PID, LOG_DAEMON);

	/* xxx 适当使用atexit() */

	if (daemonize()!=0) {
		syslog(LOG_ERR, "open(): %s", strerror(errno));
		exit(1);
	}
	syslog(LOG_INFO, "Run as a daemon now.");
	fp = fopen(FNAME, "w");
	if (fp==NULL) {
		syslog(LOG_ERR, "fopen(): %s", strerror(errno));
		exit(1);
	}
	syslog(LOG_DEBUG, "%s opedned.", FNAME);

	for (i=1;;++i) {
		fprintf(fp, "%d\n", i);
		fflush(fp);
		syslog(LOG_DEBUG, "%d written.", i);
		sleep(1);
	}

	fclose(fp);

	closelog();
	exit(0);
}

