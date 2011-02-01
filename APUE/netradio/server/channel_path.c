#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <glob.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>

#include <medialib.h>

#define PATTERN "/*.mp3"
#define SBUFSIZE 1024
#define PATSTRSIZE 128

extern int sd;

static void do_free(void *argp)
{
	free(argp);
}

static void do_close(void *darg)
{
	close((int)darg);
}

static void glob_free(void *res)
{
	globfree((glob_t *)res);
}
void *channel_pth(void *item)
{
	uint32_t i,ret,dglen;
	uint32_t fd;
	uint32_t data_per_sec = 0;
	struct timeval vtime;
	struct msg_s2c_st *msg2c = NULL;
	struct sockaddr_in his_addr;
	struct channel_item *ptr = item;
	glob_t res;
	char patstr[PATSTRSIZE];
	sigset_t set,oset;
	/* block sigals */
	sigfillset(&set);
	pthread_sigmask(SIG_UNBLOCK,&set,&oset);
	pthread_sigmask(SIG_BLOCK,&set,NULL);
	/* set address */
	his_addr.sin_family = AF_INET;
	his_addr.sin_port = htons(args.recieve_port);
	inet_pton(AF_INET,args.multicast_group,&his_addr.sin_addr);
	/* start to send data */
	memset(patstr,0,PATSTRSIZE);
	strcat(patstr,ptr->path);
	strcat(patstr,PATTERN);
	if(glob(patstr,0,NULL,&res)!=0){
		syslog(LOG_ERR,"%s\n","no mp3 item found.");
		pthread_exit(NULL);
	}
	pthread_cleanup_push(glob_free,&res);
	if(((msg2c=malloc(sizeof(*msg2c)+SBUFSIZE-1))==NULL)){
		syslog(LOG_ERR,"%s\n","failed to allocate memory.");
		globfree(&res);
		pthread_exit(NULL);
	}
	pthread_cleanup_push(do_free,msg2c);
	for(i=0;i<res.gl_pathc;){
		if((fd=open(res.gl_pathv[i],O_RDONLY))<0){
			syslog(LOG_ERR,"failed to open %s.\n",ptr->path);
			free(msg2c);
			globfree(&res);
			pthread_exit(NULL);
		}
		pthread_cleanup_push(do_close,(void*)fd);
		while(1){
			if(data_per_sec<32768){
				memset(msg2c->data,0,SBUFSIZE);
				ret=read(fd,msg2c->data,SBUFSIZE);
				data_per_sec+=ret;
				if(ret==0){
					break;
				}
				msg2c->channel_id = htons(ptr->id);
				dglen = ret + sizeof(msg2c->channel_id);
				while(sendto(sd, msg2c, dglen, 0, (void*)&his_addr, sizeof(his_addr))<0) {
					if(errno==EINTR){
						continue;
					}
					syslog(LOG_ERR,"%s\n","failed to send message.");
					free(msg2c);
					globfree(&res);
					pthread_exit(NULL);
				}
			}else {
				/* init time */
				vtime.tv_sec = 1;
				vtime.tv_usec = 0;
				select(0,NULL,NULL,NULL,&vtime);
				data_per_sec = 0;
			}
		}
		pthread_cleanup_pop(1);
		i++;
		if(i==res.gl_pathc){
			i = 0;
		}
	}
	pthread_sigmask(SIG_SETMASK,&oset,NULL);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	syslog(LOG_ERR,"i=%d\t%s\n",i,"exception exit.");
	pthread_exit(NULL);
}
