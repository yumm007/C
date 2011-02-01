#ifndef	P_MSG_H
#define	P_MSG_H

#define	MAXLEN	1024
#define	KEYPATH	"/etc/hosts"
#define	KEYPROJ	's'
#define	KEYREQ	'A'

enum {
	REQ = 1,
	STP
}; /*	mtype */

struct msg_st {
	long 			mtype; /* = msg_identify */
	unsigned		msg_identify; /* pid */
	pthread_t	msg_pthd;
	off_t			msg_len;
	char 			content[MAXLEN];
};

#endif
