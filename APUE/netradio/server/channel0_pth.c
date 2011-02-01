/***********************************************************************************
  -------------- 0号频道线程, 参数为链表头,定时遍历链表, 广播频道信息
  -------------- void *channel_0_pth(const struct channel_item_st *head)
 ***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <syslog.h>
#include <arpa/inet.h>
#include <string.h>
#include <medialib.h>
////////////////////////////


#define BUFSIZE 10

static void pth_free_buf(void *bufp)
{
	free(bufp);
}


void *channel_0_pth(void *data)
{
	struct channel_item_st *head,*channelp = data;    /*----head*/
	static struct msg_s2c_st *tmp ,*sbufp = NULL;
	static int len,i,num,err;
	static char buf[BUFSIZE],*err_buf,arr;
	static struct sockaddr_in server_addr;
	static socklen_t socket_len;
	static sigset_t nset, saveset;
	static struct timeval time_s;
	/*此线程屏蔽所有信号*/
	sigfillset(&nset);
	pthread_sigmask(SIG_BLOCK, &nset, &saveset);

	pthread_cleanup_push(pth_free_buf,(void *)sbufp);
	while(1){
		
		/*遍历频道链表*/
		head = data;
		while(channelp->next != NULL){
			channelp = channelp->next;
		}			
		
		arr = '\n';
		num = channelp->channel_info.id + 1;
		/*申请空间*/
		tmp = sbufp;
		sbufp = realloc( sbufp, (sizeof(struct msg_s2c_st)+DESCLEN)*num);
		if(sbufp == NULL){
			syslog(LOG_PERROR,"realloc()");
		}
		memset(sbufp->data,0,(sizeof(struct msg_s2c_st)+DESCLEN)*num);
#if 0
		if((long int)tmp != (long int)sbufp){
			pthread_cleanup_pop(0);	
			pthread_cleanup_push(pth_free_buf,(void *)sbufp);
		}
#endif
		sbufp->data[0] = '\0';
		for(i = 1;i < num;i++){
			snprintf(buf,8,"%d:",i);
			strcat((char *)sbufp->data,buf);  
			strcat((char *)sbufp->data,head->channel_info.desc);
			strcat((char *)sbufp->data,&arr);
			if(head->next != NULL){
				head = head->next;
			}
			
		}
		len = (sizeof(struct msg_s2c_st)+DESCLEN)*num;

		/*设定socket地址和端口*/
		server_addr.sin_family = AF_INET;
		//server_addr.sin_port = htons(args.recieve_port);
		//inet_pton(AF_INET, args.multicast_group, &server_addr.sin_addr);
		server_addr.sin_port = htons(DEFAULT_RCVPORT);
		inet_pton(AF_INET, DEFAULT_MGROUP, &server_addr.sin_addr);
		socket_len = sizeof(struct sockaddr_in);


		/*发送到socket*/
		if ((err = sendto(sd, sbufp, len, 0, (void*)&server_addr, socket_len))<0) {
			err_buf = strerror(err);
			syslog(LOG_ERR,"%s\n",err_buf);
			continue; 
		}

		/*休眠1秒，继续发送*/
		time_s.tv_sec = 1;
		time_s.tv_usec = 0;
		if((err=select(0,NULL,NULL,NULL,&time_s))<0){
			err_buf = strerror(err);
			syslog(LOG_ERR,"%s\n",err_buf);
			continue; 
		}

	}
	pthread_cleanup_pop(1);
	pthread_exit(NULL);
}

