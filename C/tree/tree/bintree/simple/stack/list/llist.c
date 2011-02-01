#include <stdlib.h>
#include <string.h>

#include "llist.h"

#if 0
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
#endif

LLIST *llist_creat(int size)
{
      LLIST *new;

      new = malloc(sizeof(*new));
      if (new == NULL) {
	    return NULL;
      }

      new->size = size;
      new->head.next = new->head.prev = &new->head;

      return new;
}

void llist_destroy(LLIST *ptr)
{

}

int llist_insert(LLIST *ptr, void *data, int mode)
{
      struct llist_node_st *newnode;

      newnode = malloc(sizeof(*newnode) + ptr->size);
      if (newnode == NULL) {
	    return -1;
      }
      memcpy(newnode->data, data, ptr->size);

      if (mode == LLIST_FORWARD) {
	    newnode->next = ptr->head.next;
	    newnode->prev = &ptr->head;
      } else if (mode == LLIST_BACKWARD) {
	    newnode->next = &ptr->head;
	    newnode->prev = ptr->head.prev;
      }
      newnode->next->prev = newnode;
      newnode->prev->next = newnode;

      return 0;
}

static struct llist_node_st *__find(LLIST *ptr, void *key, llist_cmp *cmp, int mode)
{
      struct llist_node_st *cur;

      if (mode == LLIST_FORWARD) {
	    for (cur = ptr->head.next; cur != &ptr->head; cur = cur->next) {
		  if (!cmp(key, cur->data)) {
			break;
		  }
	    }
      } else if (mode == LLIST_BACKWARD) {
	    for (cur = ptr->head.prev; cur != &ptr->head; cur = cur->prev) {
		  if (!cmp(key, cur->data)) {
			break;
		  }
	    }
      }

      return cur;
}

void llist_delet(LLIST *ptr, void *key, llist_cmp *cmp, int mode)
{
      struct llist_node_st *cur;

      cur = __find(ptr, key, cmp, mode);
      if (cur == &ptr->head) {
	    return;
      }

      cur->next->prev = cur->prev;
      cur->prev->next = cur->next;

      free(cur);
}

void *llist_find(LLIST *ptr, void *key, llist_cmp *cmp, int mode)
{
      struct llist_node_st *node;

      node =  __find(ptr, key, cmp, mode);

      if (node == &ptr->head) {
	    return NULL;
      }
      return node->data;
}

void llist_travel(LLIST *ptr, llist_op *op)
{
      struct llist_node_st *cur;

      for (cur = ptr->head.next; cur != &ptr->head; cur = cur->next) {
	    op(cur->data);
      }
}

int llist_fetch(LLIST *ptr, void *data, void *key, llist_cmp *cmp, int mode)
{
      struct llist_node_st *node;

      node =  __find(ptr, key, cmp, mode);

      if (node == &ptr->head) {
	    return -1;
      }

      memcpy(data, node->data, ptr->size);

      node->next->prev = node->prev;
      node->prev->next = node->next;
      free(node);

      return 0;
}
