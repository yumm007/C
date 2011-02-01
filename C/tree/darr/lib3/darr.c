#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "darr.h"

#define BUF_NUM 5

struct darr_st {
      int size;
      int num;
      int total;
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
      new->total = 0;
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

      if (ptr->num == ptr->total) {
	    tmp = realloc(ptr->arr, ptr->size * (ptr->total + BUF_NUM));
	    if (tmp == NULL) {
		  return -1;
	    }
	    ptr->arr = tmp;
	    ptr->total += BUF_NUM;
      }

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
		  if (ptr->num == ptr->total - BUF_NUM) {
			ptr->total -= BUF_NUM;
			ptr->arr = realloc(ptr->arr, ptr->size * ptr->total);
			if (ptr->total == 0) {
			      ptr->arr = NULL;
			}
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

int darr_save(DARR *darr, const char *path)
{
      struct darr_st *ptr = darr;
      FILE *fp;
      int ret;

      fp = fopen(path, "w");
      if (fp == NULL) {
	    return -1;
      }

      ret = fwrite(&ptr->size, sizeof(ptr->size), 1, fp);
      if (ret < 1) {
	    return -1;
      }
      ret = fwrite(&ptr->num, sizeof(ptr->num), 1, fp);
      if (ret < 1) {
	    return -1;
      }

      ret = 0;
      while (1) {
	    ret += fwrite(ptr->arr + ptr->size * ret, ptr->size, ptr->num - ret, fp);
	    if (ret == ptr->num) {
		  break;
	    }
	    if (ret == 0) {
		  /* if full */
	    }
      }

      fclose(fp);
      return 0;
}

DARR *darr_load(const char *path)
{
      FILE *fp;
      struct darr_st *new;
      int ret;

      fp = fopen(path, "r");
      if (fp == NULL) {
	    return NULL;
      }

      new = malloc(sizeof(*new));
      if (new == NULL) {
	    fclose(fp);
	    return NULL;
      }

      ret = fread(&new->size, sizeof(new->size), 1, fp);
      if (ret < 1) {
	    free(new);
	    fclose(fp);
	    return NULL;
      }
      ret = fread(&new->num, sizeof(new->num), 1, fp);
      if (ret < 1) {
	    free(new);
	    fclose(fp);
	    return NULL;
      }

      new->arr = malloc(new->size * new->num);
      if (new->arr == NULL) {
	    free(new);
	    fclose(fp);
	    return NULL;
      }

      ret = fread(new->arr, new->size, new->num, fp);
      if (ret < new->num) {
	    free(new->arr);
	    free(new);
	    fclose(fp);
	    return NULL;
      }

      fclose(fp);
      return new;
}

