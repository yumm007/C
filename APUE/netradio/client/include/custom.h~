#ifndef CUSTOM_H
#define CUSTOM_H

#include <comm.h>

#define DEFAULT_MSGBUF_AMOUNTMIN 20

struct arg_st{
	char 		*player_command;
	unsigned int 	recieve_port;
	char 		*multicast_group;
};

extern struct arg_st args;

void player_cancel(int s);
int  init_socket(const char* group, int port);
void player(int fd);
void set_msgbuf_amountmin(int amount);
int change_arg(struct arg_st *arg, int argc, char **argv);

#endif


