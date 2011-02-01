#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <pthread.h>

#include "link.h"

struct link_node {
	void *data;
	struct link_node *prev, *next;
};

struct link_head {
	size_t num;
	size_t node_size;
	cmp_fun ins_cmp, del_cmp;
	proc_fun emp_fun;
	pthread_mutex_t link_mut;
	struct link_node head;
};

void *link_init(size_t size, cmp_fun ins_cmp, cmp_fun del_cmp, proc_fun emp_fun) {
	struct link_head *ret = NULL;
	
	if ((ret = malloc(sizeof *ret)) != NULL) {
		ret->num = 0;
		ret->node_size = size;
		ret->ins_cmp = ins_cmp;
		ret->del_cmp = del_cmp;
		ret->emp_fun = emp_fun;
		pthread_mutex_init(&ret->link_mut, NULL);
		ret->head.prev = ret->head.next = &ret->head;
		ret->head.data = NULL;
	}
	
	return ret;
}

void link_erase(void *head) {
	struct link_node *cur = NULL, *tmp = NULL;
	struct link_head *link = head;
	
	if (head == NULL)
		return;

	for (cur = link->head.next; cur != &link->head; cur = tmp) {
			tmp = cur->next;
			link->emp_fun(cur->data);
			free(cur->data);
			free(cur);
	}

	pthread_mutex_destroy(&link->link_mut);
	free(head);
	return;
}

int link_add(const void *node, void *head) {
	struct link_node *cur = NULL, *tmp = NULL;
	struct link_head *link = head;
	
	if ((tmp = malloc(sizeof(*tmp))) == NULL)
		return -1;
	if ((tmp->data = malloc(link->node_size)) == NULL) {
		free(tmp);
		return -1;
	}
	memcpy(tmp->data, node, link->node_size);

	pthread_mutex_lock(&link->link_mut);
	for (cur = link->head.next; cur != &link->head; cur = cur->next)
		if ((link->ins_cmp)(node, cur->data) < 0)
			break;
		
	/*将tmp插入到cur之前*/
	tmp->prev = cur->prev;
	tmp->next = cur;
	tmp->prev->next = tmp;
	tmp->next->prev = tmp;

	pthread_mutex_unlock(&link->link_mut);

	return ++link->num;
}

int link_del(const void *node, void *head) {
	struct link_node *cur = NULL;
	struct link_head *link = head;
	
	pthread_mutex_lock(&link->link_mut);
	for (cur = link->head.next; cur != &link->head; cur = cur->next) 
		if ((link->del_cmp)(node, cur->data) == 0) {
			cur->next->prev = cur->prev;
			cur->prev->next = cur->next;
			pthread_mutex_unlock(&link->link_mut);

			link->emp_fun(cur->data);
			link->num--;
			free(cur->data);
			free(cur);
			return 0;
		}

	pthread_mutex_unlock(&link->link_mut);
	return -1;
}

void link_travel(const void *head, op_fun fun) {
	struct link_node *cur = NULL;
	const struct link_head *link = head;
	
	for (cur = link->head.next; cur != &link->head; cur = cur->next) 
		fun(cur->data);
}

void *link_each(const void *head, void **save) {
	struct link_node *next;
	const struct link_head *link = head;

	if (*save == NULL) {
		*save = link->head.next;
		return link->head.next->data;
	}

	next = *(struct link_node **)save;
	if ((next = next->next) != &link->head){
		*save = next;
		return next->data;
	}
	return NULL;
}	

