#ifndef	P_MSG_H
#define	P_MSG_H

#define	SERVER	"127.0.0.1"
#define	CLIENT	"127.0.0.1"
#define	REQPT		11233

#define	MAXLEN			256
#define	TIME_TO_EXIT	5

enum {
	REQ = 1,
	STP,
	REC,
}; /*	mtype */

struct msg_st {
	u_int32_t 	mtype;
	u_int32_t	msg_len;  /* 32位地址偏移量*/
	u_int8_t 	content[MAXLEN];
} __attribute__((packed));

#endif
