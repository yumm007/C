#ifndef COMM_H
#define COMM_H
#include <sys/types.h>

#define DEFAULT_RCVPORT 18761
#define DEFAULT_MGROUP "224.2.4.5"

typedef u_int16_t channel_id_t;

struct msg_s2c_st {
	channel_id_t channel_id;
	u_int8_t data[1];
};
/*
如果是频道列表：
%d:%s\r\n
%d:%s\r\n
%d:%s\r\n
...
*/

#endif
