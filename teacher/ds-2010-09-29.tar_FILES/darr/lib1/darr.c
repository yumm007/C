#include <stdlib.h>
#include <string.h>

#include "darr.h"

#if 0
typedef struct {
      int size;
      int num;
      char *arr;
} DARR;
typedef int darr_cmp(const void *, const void *);
typedef void darr_op(void *);
#endif

DARR *darr_creat(int size)
{
      DARR *new;

      new = malloc(sizeof(*new));
      if (new == NULL) {
	    return NULL;
      }

      new->size = size;
      new->num = 0;
      new->arr = NULL;

      return new;
}

void darr_destroy(DARR *ptr)
{
      free(ptr->arr);
      free(ptr);
}

int darr_append(DARR *ptr, void *data)
{
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

void *darr_find(DARR *ptr, void *key, darr_cmp *cmp)
{
      int i;

      for (i = 0; i < ptr->num; i++) {
	    if (!cmp(key, ptr->arr + ptr->size * i)) {
		  return ptr->arr + ptr->size * i;
	    }
      }

      return NULL;
}

void darr_delet(DARR *ptr, void *key, darr_cmp *cmp)
{
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

void darr_travel(DARR *ptr, darr_op *op)
{
      int i;

      for (i = 0; i < ptr->num; i++) {
	    op(ptr->arr + ptr->size * i);
      }
}

