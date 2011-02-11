#ifndef HS_DARR_H
#define HS_DARR_H

typedef struct {
      int size;
      int num;
      char *arr;
} DARR;

typedef int darr_cmp(const void *, const void *);
typedef void darr_op(void *);

DARR *darr_creat(int size);

void darr_destroy(DARR *);

int darr_append(DARR *, void *data);

void *darr_find(DARR *, void *key, darr_cmp *cmp);

void darr_delet(DARR *, void *key, darr_cmp *cmp);

void darr_travel(DARR *, darr_op *op);

/* homework */
darr_fetch();

#endif
