#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <custom.h>

#define PLAYER DEFAULT_PLAYER" -"

static void print_help(void) {
	system("cat /usr/local/netradio/doc/client_help");
}

int change_arg(struct arg_st *arg, int argc, char **argv)
{
	int ch;
	int help_flag = 0;
	
	arg->player_command = PLAYER;
	arg->recieve_port = DEFAULT_RCVPORT;
	arg->multicast_group = DEFAULT_MGROUP;

	while((ch = getopt(argc, argv,  "-m:p:g:h")) != -1){
		switch(ch){
			case 'm':
				arg->player_command = optarg;
				break;
			case 'p':
				arg->recieve_port = atoi(optarg);
				break;
			case 'g':
				arg->multicast_group = optarg;
				break;
			case 'h':
				print_help();
				help_flag = 1;
				break;
			default:
				print_help();
				help_flag = 1;
				break;
		}
	}
	
	if (help_flag == 1)
		return 1;
	return 0;
}

