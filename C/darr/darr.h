#ifndef _HS_DARR_H
#define _HS_DARR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSTEP 10

typedef struct {
	int num;
	int size;
	char *droot;
} DARR;

void *__realloc(DARR *darr, int flag);
int save_darr(const DARR *darr, const char *filepath);
DARR *load_darr(const char *filepath);
DARR *create_darr(size_t size);
int append_darr(DARR *darr, void *data);
void *find_node(DARR *darr, void *key, int (*cmp)(const void *data, const void *key));
void delete_node(DARR *darr, void *key, int(*cmp)(const void *data, const void *key));
//int update_node(void *data, const void *newdata, size_t size);
int inserd_node(DARR *darr, const void *data, int i);
void *fetch(DARR *darr, void *key, int(*cmp)(void *data, void *key));
void erase(DARR *darr);
void travel(DARR *darr, void (*op)(void *data));
void darr_sort(DARR *darr, int (*cmp_fun)(const void *data1, const void *data2));

#endif
