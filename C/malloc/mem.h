#ifndef _HS_MEM_H
#define _HS_MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct free_mem_node {
	struct free_mem_node *prev;
	struct free_mem_node *next;
	size_t size;
};

typedef struct {
	size_t num;
	struct free_mem_node head;
} MEMLLIST;

MEMLLIST *init_mem(void *addr, size_t size);
void *request_mem(MEMLLIST *llist, size_t size);
void free_mem(MEMLLIST *llist, void *addr);
void travel_free_mem(MEMLLIST *llist);
void * resize_mem(MEMLLIST *llist, void *addr, size_t newsize);

#endif
