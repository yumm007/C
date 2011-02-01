#include <stdlib.h>
#include <string.h>

#include "darr.h"

struct darr_st {
      int size;
      int num;
      char *arr;
};

#if 0
typedef int darr_cmp(const void *, const void *);
typedef void darr_op(void *);
#endif

DARR *darr_creat(int size)
{
      struct darr_st *new;

      new = malloc(sizeof(*new));
      if (new == NULL) {
	    return NULL;
      }

      new->size = size;
      new->num = 0;
      new->arr = NULL;

      return new;
}

void darr_destroy(DARR *darr)
{
      struct darr_st *ptr = darr;

      free(ptr->arr);
      free(ptr);
}

int darr_append(DARR *darr, void *data)
{
      struct darr_st *ptr = darr;
      void *tmp;

      tmp = realloc(ptr->arr, ptr->size * (ptr->num + 1));
      if (tmp == NULL) {
	    return -1;
      }
      ptr->arr = tmp;

      memcpy(ptr->arr + ptr->size * ptr->num, data, ptr->size);
      ptr->num++;

      return 0;
}

void *darr_find(DARR *darr, void *key, darr_cmp *cmp)
{
      struct darr_st *ptr = darr;
      int i;

      for (i = 0; i < ptr->num; i++) {
	    if (!cmp(key, ptr->arr + ptr->size * i)) {
		  return ptr->arr + ptr->size * i;
	    }
      }

      return NULL;
}

void darr_delet(DARR *darr, void *key, darr_cmp *cmp)
{
      struct darr_st *ptr = darr;
      int i;

      for (i = 0; i < ptr->num; i++) {
	    if (!cmp(key, ptr->arr + ptr->size * i)) {
		  memmove(ptr->arr + ptr->size * i,  ptr->arr + ptr->size * (i + 1), ptr->size * (ptr->num - i - 1));
		  ptr->num--;
		  ptr->arr = realloc(ptr->arr, ptr->size * ptr->num);
		  if (ptr->num == 0) {
			ptr->arr = NULL;
		  }
		  return;
	    }
      }
}

void darr_travel(DARR *darr, darr_op *op)
{
      struct darr_st *ptr = darr;
      int i;

      for (i = 0; i < ptr->num; i++) {
	    op(ptr->arr + ptr->size * i);
      }
}

