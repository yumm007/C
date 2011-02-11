#ifndef HS_LLIST_H
#define HS_LLIST_H

#define LLIST_FORWARD  1
#define LLIST_BACKWARD 2

typedef int llist_cmp(const void *, const void *);
typedef void llist_op(void *);

struct llist_node_st {
      struct llist_node_st *prev, *next;
      char data[0];
};

typedef struct {
      int size;
      struct llist_node_st head;
} LLIST;

LLIST *llist_creat(int size);

void llist_destroy(LLIST *);

int llist_insert(LLIST *, void *data, int mode);

void llist_delet(LLIST *, void *key, llist_cmp *cmp, int mode);

void *llist_find(LLIST *, const void *key, llist_cmp *cmp, int mode);

void llist_travel(LLIST *, llist_op *op);

/* homework */
int llist_fetch(LLIST *, void *data, void *key, llist_cmp *cmp, int mode);

int llist_getnum(LLIST *);

#endif
