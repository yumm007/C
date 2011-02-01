#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hash_link.h"

struct link_st {
	size_t	data_size;
	void 		*data;
	struct	link_st *next;
};

struct arr_st {
	pthread_mutex_t 	arr_mute;
	struct link_st 	*link;
};

struct arr_head {
	size_t 			arr_size;
	cmp_fun			cmp;
	struct arr_st 	*arr_addr;
};

HASHLINK *link_init(size_t hash_size, cmp_fun fun) {
	struct arr_st *arr = NULL;
	struct arr_head *head = NULL;
	char *tmp;
	size_t i;

	if ((tmp = malloc(sizeof(*head) + hash_size * sizeof(*arr))) == NULL)
		return NULL;
	head = (void *)tmp;
	head->arr_size = hash_size;
	head->arr_addr = arr = (void *)(head + 1);
	head->cmp = fun;

	for (i = 0; i < hash_size; i++) {
		pthread_mutex_init(&arr[i].arr_mute, NULL);
		arr[i].link = NULL;
	}

	return head;
}

static void free_arr_st(struct link_st *link) {
	struct link_st *cur, *tmp;
	for (cur = link; cur != NULL; cur = tmp) {
		tmp = cur->next;
		free(cur->data);
		free(cur);
	}
}

void link_dstr(HASHLINK *link) {
	struct arr_head *head = link;
	struct arr_st *arr;
	size_t i;
	
	arr = head->arr_addr; 
	for (i = 0; i < head->arr_size; i++) {
		free_arr_st(arr[i].link);
		pthread_mutex_destroy(&arr[i].arr_mute);
	}
	free(head);
}

static int link_insert(struct arr_st *head, const void *data, size_t data_size) {
	struct link_st *tmp_st, **cur_link; 
	void *tmp_data;

	if ((tmp_st = malloc(sizeof(*tmp_st))) == NULL)
		return -1;
	if ((tmp_data = malloc(data_size)) == NULL) {
		free(tmp_st);
		return -1;
	}
	memcpy(tmp_data, data, data_size);
	tmp_st->data_size = data_size;
	tmp_st->data = tmp_data;
	tmp_st->next = NULL;

	cur_link = &head->link;
	pthread_mutex_lock(&head->arr_mute);
	while (*cur_link != NULL)
		*cur_link = (*cur_link)->next;

	if (*cur_link == head->link)
		head->link = tmp_st;
	else
		*cur_link = tmp_st;
	pthread_mutex_unlock(&head->arr_mute);

	return 0;
}

static void *link_find(struct arr_st *head, const void *key, size_t *data_size, cmp_fun cmp) {
	struct link_st *cur_link = head->link; 

	pthread_mutex_lock(&head->arr_mute);
	while (cur_link != NULL) {
		if (cmp(cur_link->data, key) == 0)
			break;
		cur_link = cur_link->next;
	}
	pthread_mutex_unlock(&head->arr_mute);

	if (cur_link == NULL)
		return NULL;
	*data_size = cur_link->data_size;
	return cur_link->data;
}

static int link_del(struct arr_st *head, const void *key, cmp_fun cmp) {
	struct link_st **prv, *cur_link; 

	prv = &head->link;
	pthread_mutex_lock(&head->arr_mute);
	cur_link = head->link;
	while (cur_link != NULL) {
		if (cmp(cur_link->data, key) == 0) {
			*prv = cur_link->next;
			free(cur_link->data);
			free(cur_link);
			pthread_mutex_unlock(&head->arr_mute);
			return 0;
		}
		prv = &(cur_link);
		cur_link = cur_link->next;
	}
	pthread_mutex_unlock(&head->arr_mute);
	return -1;
}

static size_t get_index(const void *key, size_t key_size){
	size_t i, sum = 0;
	char *c = (char *)key;
	for (i = 0; i < key_size; i++)
		sum = (sum << 5) ^ c[i];
	return sum;
}

int link_add_node(const HASHLINK *link,const HL_DATA *data) {
	struct arr_head *head = (void *)link;
	struct arr_st *arr;
	size_t i;

	arr = head->arr_addr; 
	i=get_index(data->key,data->key_size) % head->arr_size;
	return link_insert(arr+i, data->data, data->data_size);
}

int link_fnd_node(const HASHLINK *link, HL_DATA *data){
	struct arr_head *head = (void *)link;
	struct arr_st *arr;
	size_t i;
	void *tmp;

	arr = head->arr_addr; 
	i=get_index(data->key,data->key_size) % head->arr_size;
	tmp = link_find(arr+i, data->key, &data->data_size, head->cmp);
	if (tmp == NULL)
		return -1;
	data->data = tmp;
	return 0;
}

int link_del_node(const HASHLINK *link, HL_DATA *data){
	struct arr_head *head = (void *)link;
	struct arr_st *arr;
	size_t i;

	arr = head->arr_addr; 
	i=get_index(data->key,data->key_size) % head->arr_size;
	return link_del(arr+i, data->key, head->cmp);
}

