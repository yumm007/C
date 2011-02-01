#ifndef PTH_QUEQU_H
#define PTH_QUEQU_H

#define	PTHQ_BUF		256

typedef void PTHQ;

PTHQ *pthq_open(size_t bufsize);
int pthq_close_wr(PTHQ *);
int pthq_delete(PTHQ *);
size_t pthq_read(PTHQ *pth, void *buf, size_t bufsize);
int pthq_write(PTHQ *, const void *data, size_t data_size);

enum {E_CLSD_UN = -1, E_CLSD_Q = -2, E_MALLOC = -3,};
#endif

