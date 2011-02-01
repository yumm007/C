#ifndef RELAYER_H
#define RELAYER_H

#define RELAYER_JOBMAX 4096

#define	E_REL_FULL	-1
#define	E_REL_INIT	-4
#define	E_REL_NF		-2
#define	E_REL_NU		-3

struct relayer_stat_st {
	int fd1, fd2;
	int64_t count12, count21;
	time_t start;
	int over;
};

int relayer_addjob(int fd1, int fd2, int flags);
int relayer_statjob(int, struct relayer_stat_st*);
int relayer_canceljob(int);
int relayer_waitjob(int);
void relayer_finished(void);

#endif
