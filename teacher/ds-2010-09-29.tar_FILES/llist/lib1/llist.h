#ifndef HS_LLIST_H
#define HS_LLIST_H

#define LLIST_FORWARD  1
#define LLIST_BACKWARD 2

typedef int llist_cmp(const void *, const void *);
typedef void llist_op(void *);

struct llist_node_st {
      void *data;
      struct llist_node_st *prev, *next;
};

typedef struct {
      int size;
      struct llist_node_st head;
} LLIST;

LLIST *llist_creat(int size);

void llist_destroy(LLIST *);

int llist_insert(LLIST *, void *data, int mode);

void llist_delet(LLIST *, void *key, llist_cmp *cmp, int mode);

void *llist_find(LLIST *, void *key, llist_cmp *cmp, int mode);

void llist_travel(LLIST *, llist_op *op);

/* homework */
//llist_fetch();

#endif
