#ifndef _HS_LLIST_H
#define _HS_LLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NEXT 1
#define PREV 2


struct list_node {
	ssize_t size;
	void *data; 
	struct list_node *prev;
	struct list_node *next;
};

typedef struct {
	int num;
	struct list_node head;
} LLIST;

typedef int cmp_fun(const void *data, const void *key);

int save_llist(const LLIST *llist, const char *filepath);
LLIST *load_llist(const char *filepath);
LLIST *create_llist(void);
int insert_llist(LLIST *llist, const void *data, const size_t size, const int mode);
void *find_node(LLIST *llist, const void *key, cmp_fun *cmp, const int mode);
int delete_node(LLIST *llist, const void *key, cmp_fun *cmp, const int mode);
int update_node(LLIST *llist, const void *data, const void *newdata, const size_t size, cmp_fun cmp);
int inserd_node(LLIST *llist, const void *data, int i);
void *fetch(LLIST *llist, const void *key, cmp_fun *cmp, const int mode);
void erase(LLIST *llist);
void travel(LLIST *llist, void (*op)(void *data));
int llist_sort(LLIST *llist, int (*fun)(const void *, const void*), void (*op)(void *data));

#endif
