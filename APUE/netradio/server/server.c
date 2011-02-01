#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <syslog.h>
#include <errno.h>
#include <signal.h>

#include <medialib.h>

#define	TIME_TO_EXIT	0
#define	LOCK_FILE 	"/var/lock/netradio/pid.lck"

int sd = -1;
struct arg_st args;

static pthread_cond_t 	wait_cond = PTHREAD_COND_INITIALIZER; 
static pthread_mutex_t 	wait_mut = PTHREAD_MUTEX_INITIALIZER;
static int sig_type = -1;

static void clean_pth(struct channel_item_st *link, int count) {
	int i;
	for (i = 0; i < count; i++, link = link->next) {
		pthread_cancel(link->channel_pthd);
		pthread_join(link->channel_pthd, NULL);
	}
	medialib_free_channellist(link);
}

static int create_channel_pthread(struct channel_item_st *link) {
	int i = 0;
	pthread_t pthd;
	int ret;

	while (link != NULL) {
		ret= pthread_create(&pthd, NULL, channel_pth, &link->channel_info);
		if (ret == EAGAIN)
			continue;
		else if (ret != 0) {
			clean_pth(link, i);
			return -1;
		}
		i++;
		link->channel_pthd = pthd;
		link = link->next;
	}

	return 0;
}

static void wait_until_exit(struct channel_item_st *link, int count) {
	pthread_mutex_lock(&wait_mut);
	while (sig_type != TIME_TO_EXIT) {
		pthread_cond_wait(&wait_cond, &wait_mut);
		switch (sig_type) {
			case TIME_TO_EXIT:
				clean_pth(link, count);
				break;
			default:
				break;
		}
	}
	pthread_mutex_unlock(&wait_mut);

	return;
}

static void sig_hand(int unused) {
	pthread_mutex_lock(&wait_mut);
	sig_type = TIME_TO_EXIT;
	syslog(LOG_INFO, "recived SIGINT, exit.");
	pthread_cond_broadcast(&wait_cond);
	pthread_mutex_unlock(&wait_mut);
}

static int install_signal(void) {
	struct sigaction sig;
	sigset_t newsig, oldsig;

	sigfillset(&newsig);
	sigprocmask(SIG_BLOCK, &newsig, &oldsig);

	sig.sa_handler = sig_hand;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;

	if (sigaction(SIGINT, &sig, NULL) != 0)
		return -1;

	sigemptyset(&oldsig);
	sigaddset(&oldsig, SIGINT);
	sigprocmask(SIG_UNBLOCK, &oldsig, NULL);

	return 0;
}

static int create_socket(char *group) {
	struct ip_mreqn mreq;
	if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) 
    	  return -1;

	inet_pton(AF_INET, group, &mreq.imr_multiaddr);
   	inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
   	mreq.imr_ifindex = if_nametoindex("eth0");
   	if (setsockopt(sd, IPPROTO_IP,IP_MULTICAST_IF,&mreq,sizeof(mreq))<0) {
		close(sd);
     	return -1;
   	}   

	return sd;
}

static int save_pid(const char *filepath) {
	FILE *fp;

	if ((fp = fopen(filepath, "w")) == NULL) {
		syslog(LOG_ERR, "open file %s failed.", filepath);
		return -1;
	}

	fprintf(fp, "%u", (unsigned int)getpid());
	fclose(fp);
	return 0;
}

static void my_daemon(void) {
	pid_t pid;
	int fd;
	
	if ((pid = fork()) < 0) {
		syslog(LOG_ERR, "fork failed, exit.");
		closelog();
		exit(1);
	} else if (pid != 0) {
		closelog();
		exit(0);
	}

	fd = open("/dev/NULL", O_RDWR);
		
	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);

	if (fd > 2)
		close(fd);

	chdir("/");
	umask(0);
	setsid();

	return;
}

int main(int argc, char *argv[])
{
	int chnr = 0;
	struct channel_item_st *link = NULL;
	pthread_t pthd;

	openlog("netradio", LOG_PID, LOG_DAEMON);
	syslog(LOG_INFO, "daemon start.");

	sep_arg(&args);
	my_daemon();

	if (install_signal() != 0) {
		syslog(LOG_ERR, "install signal failed, exit.");
		closelog();
		exit(1);
	}

	if (save_pid(LOCK_FILE) == -1) {
		closelog();
		exit(1);
	}

	if ((sd = create_socket(args.multicast_group)) == -1) {
		syslog(LOG_ERR, "create socket failed, exit.");
		closelog();
		exit(1);
	}

	if (medialib_get_channellist(&chnr, &link) != 0) {
		syslog(LOG_ERR, "init channellist failed, exit.");
		closelog();
		close(sd);
		exit(1);
	}

	if (pthread_create(&pthd, NULL, channel_0_pth, link) != 0) {
		syslog(LOG_ERR, "init channel 0 failed, exit.");
		medialib_free_channellist(link);
		closelog();
		close(sd);
		exit(1);
	}

	//失败时会释放link 和已经打开的线程
	if (create_channel_pthread(link) == -1) {
		syslog(LOG_ERR, "init channel pthread failed, exit.");
		closelog();
		close(sd);
		exit(1);
	}

	wait_until_exit(link, chnr);

	pthread_cancel(pthd); /*cancel pth 0*/
	pthread_join(pthd, NULL);

	syslog(LOG_INFO, "netradio daemon exit.");
	closelog();
	close(sd);

	return 0;
}

