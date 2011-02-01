#ifndef MEDIALIB_H
#define MEDIALIB_H

#include <comm.h>

#define DEFAULT_DESCFILE "desc.text"

#define PATHLEN	1024
#define DESCLEN	1024
#define MPATHSIZE 1024
#define PORTSIZE 6
#define MULTICASTGRPSIZ 16

struct channel_item {
	channel_id_t 	id;
	char 				path[PATHLEN];
	char				desc[DESCLEN];
};

struct channel_item_st {
	pthread_t		channel_pthd;
	struct channel_item	channel_info;
   struct			channel_item_st *next;
};

struct arg_st{
	char media_path[MPATHSIZE];
	unsigned int recieve_port;
	char multicast_group[MULTICASTGRPSIZ];
};

extern struct arg_st args;
extern int sd;

int medialib_get_channellist(int *count, struct channel_item_st **head);
int medialib_free_channellist(struct channel_item_st *head);
void *channel_pth(void *item);
void *channel_0_pth(void *head);
void sep_arg(struct arg_st *argp);

#endif

