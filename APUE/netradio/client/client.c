#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <sys/select.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <custom.h>

#define RCVADDR "0.0.0.0"
#define BUFSIZE 1024

typedef struct msg_s2c_st msg_s2c;

int msgbuf_amountmin = DEFAULT_MSGBUF_AMOUNTMIN;

static int time_to_exit;
static int sd;
static int pd[2];
static pthread_t thr;
static int first_flag = 1;
static int channel_id;

/*********** set amount  ***********/
void set_msgbuf_amountmin(int amount) {
	msgbuf_amountmin = amount;
}

/*********** cancel self ***********/
void player_cancel(int s) {
	time_to_exit = 1;
	close(sd);
	close(pd[0]);
	close(pd[1]);
}

/***********   init sd   ***********/
int init_socket(const char *group, int port) {
	struct ip_mreqn mreq;
	struct sockaddr_in my_addr;
	int val;

	sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sd < 0) {
		perror("client.c:socket()");
		return -1;
	}   
//---------------------add multigroup---------------
	inet_pton(AF_INET, group, &mreq.imr_multiaddr);
	inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);   
	mreq.imr_ifindex = if_nametoindex("eth0");
	if (setsockopt(sd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))<0){
		close(sd);
		return -1;
	}

//----------------------bind-------------------------
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	inet_pton(AF_INET, RCVADDR, &my_addr.sin_addr);
	if (bind(sd, (void*)&my_addr, sizeof(my_addr))<0) {
		perror("client.c:bind()");
		close(sd);
		return -1;
	}

//---------------when--you--reuse,-----------
	val=1;
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val))<0) {
		perror("setsockopt()");
		close(sd);
		return -1;
	}

//---------------  init pipe ----------------
	if (pipe(pd) < 0) {
		perror("client.c:pipe()");
		close(sd);
		return -1;
	}

	return 0;
}

/*********** rcv information from sd ***********/
static int udp_rcv(msg_s2c **msg, int channel_id) {
	int msg_len;
	int err;
	while(1) {
		while(1) {
			if (ioctl(sd, FIONREAD, &msg_len) != 0) {
				perror("client.c:ioctl()");
				return -1;
			}

			if (msg_len != 0) {
				/* read a information form sd */
				break;
			}
			if (time_to_exit == 1) {
				return -2;
			}

		}

		if (time_to_exit == 1) {
			return -2;
		}

		*msg = malloc(msg_len);
		if (*msg == NULL) {
			perror("client.c:malloc()");
			return -1;
		}

		while (1) {
			if (recvfrom(sd, *msg, msg_len, 0, NULL, NULL) < 0) {
				if (err == EINTR)
					continue;
				perror("client.c:recvfrom()");
				return -1;
			}
			if (time_to_exit == 1) {
				return -2;
			}

			break; 
		}

		if (ntohs((*msg)->channel_id) == channel_id) {
			break;
		}
	}

	return msg_len - sizeof((*msg)->channel_id);
}

/*********** show list ***********/
static int select_channel(pid_t pid) {
	msg_s2c *msg = NULL;
	char *data = NULL;
	char *tav = NULL;
	unsigned int dataline;
	int ctav;
	int len, slen;

	puts("  Please wait the list from network!");
	if (udp_rcv(&msg, 0) < 0) {
		fprintf(stderr, "get channle error.\n");
		return -1;
	}

	if (data != NULL) {
		free(data);
		data = NULL;
	}
	dataline = 0;

	while (1) {
		if (time_to_exit == 1) {
			return -1;
		}

		system("clear");
		printf("\n\n+++++++++++++++++++++++++ CHANNEL INFOR +++++++++++++++++++++++++\n");
		printf("%s", msg->data);
		printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n");
		if (first_flag == 1) {
			printf("++ Choose the channel No.: ");
		} else {
			printf("++ Your are in the channel No.: %d\n\n", channel_id);
			printf("++ If your want to change the channel!\n++ Please choose the channel No.: ");
		}
		getline((char**)&data, &dataline, stdin);
		len = strlen(data) - 1;
		data[len] = '\0';

		ctav = 1;
		for (slen = len; slen > 1; slen--)
			ctav *= 10;
		channel_id = atoi(data);

		if ((tav = strstr((char*)msg->data, data)) != NULL) {
			if (tav[len] == ':')
				break;
		}

		fprintf(stderr, "Input error! Type any key to choose again!\n");
		getline((char**)&data, &dataline, stdin);
	}

	if (first_flag == 0) {
		kill(pid, SIGKILL);
		wait(NULL);
	}

	first_flag = 0;
	return channel_id;
}

/*********** the thread send data to mplayer ***********/
static void* thr_read(void* ptr) {
	struct timeval tv;
	int len, ret, pos;
	char buf[BUFSIZE];
	int fd = *(int*)ptr;

	len = 0;
	pos = 0;
	while(1) {
		if (time_to_exit == 1) {
			break;
		}

		if (len == 0) {
			if (time_to_exit == 1) {
				break;
			}

			len = read(pd[0], buf, BUFSIZE);
			if (len <= 0) {
				tv.tv_sec = 1;
				tv.tv_usec = 0;
				select(0, NULL, NULL, NULL, &tv);
			}
			continue;
		}

		if (len < 0) {
			printf("error\n");
		}

		ret = write(fd, buf+pos, len);
		if (ret <= 0) {
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			select(0, NULL, NULL, NULL, &tv);
			continue;
		}
		len -= ret;
		pos += ret;
		if (pos == ret) pos = 0;
	}
	/* tell player exit */
	time_to_exit = 1;
	pthread_exit(NULL);
}

static void thr_cancel(void* ptr) {
	pthread_cancel(thr);
	return;
}

/*********** start work ***********/
void player(int fd) {
	int id;
	pid_t pid;

	while(1) {
		id = select_channel(pid);
		if (time_to_exit == 1) {
			break;
		}
		pid = fork();
		if (pid < 0) {
			perror("client.c:fork()");
			return;
		}

		if (pid == 0) {
			msg_s2c *msg = NULL;
			int datalen, wbit;
			int msgbuf_amount = 0;
			pthread_t thr;
			while (1) {
				if ((datalen = udp_rcv(&msg, channel_id)) == -1) {
					break;
				}

				if (datalen == -2) {
					break;
				}

				msgbuf_amount++;

				wbit = 0;
				while(datalen != wbit) {
					wbit += write(pd[1], msg->data + wbit, datalen - wbit);
				}

				if (msgbuf_amount == msgbuf_amountmin) {
					pthread_create(&thr, NULL, thr_read, &fd);
					pthread_cleanup_push(thr_cancel, &thr);
					continue;
					pthread_cleanup_pop(0);
				}
			}
			exit(1);
		}
	}
	return;
}

