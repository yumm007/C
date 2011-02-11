#ifndef MYTBF_H
#define MYTBF_H

#define TBF_MAX 1024

typedef void MYTBF;

MYTBF *tbf_context_new(int cps, int burst);
int tbf_context_delete(MYTBF *);

int tbf_write(int, const char*, size_t, MYTBF *);

#endif

