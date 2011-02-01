#ifndef PTH_STACK_H
#define PTH_STACK_H

typedef void PTHS;

PTHS *pths_open(size_t stack_buf);
size_t pths_pop(PTHS *pths, void *buf, size_t buf_size);
int pths_push(PTHS *pths, const void *data, size_t data_size);
int pths_close_wr(PTHS *pths);
int pths_close(PTHS *pths);

enum {E_CLSD_UN = -1, E_CLSD_Q, E_MALLOC, E_EMPTY};

#endif

