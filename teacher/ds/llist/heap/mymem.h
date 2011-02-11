#ifndef HS_MYMEM_H
#define HS_MYMEM_H

#define MYMEM_ALIGN sizeof(int)
#define MYMEM_REMAIN 32

typedef struct mymem_nodest {
      char *end;
      struct mymem_nodest *prev, *next;
} MEM;

MEM *mymem_init(void *start, unsigned size);

void mymem_clean(MEM *);

void *mymem_alloc(MEM *, unsigned size);

void mymem_free(MEM *, void *);

#endif
