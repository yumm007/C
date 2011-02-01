#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define NEXT 2
#define PREV 1

struct list_head {
	struct list_head *next, *prev;
};

struct list_node {
   ssize_t size;
   void *data;
   struct list_head link;
};

typedef struct {
   int num;
   struct list_head head;
} LLIST;

typedef int cmp_fun(const void *data, const void *key);

LLIST *create_llist(void);
int insert_llist(LLIST *llist, const void *data, const size_t size, const int mode);
void travel(LLIST *llist, void (*op)(void *data));
int save_llist(const LLIST *llist, const char *filepath);
LLIST *load_llist(const char *filepath);
struct list_node * __find_node(LLIST *llist, const void *key, cmp_fun *cmp, int mode);
void *find_node(LLIST *llist, const void *key, cmp_fun *cmp, int mode);
int delete_node(LLIST *llist, const void *key, cmp_fun *cmp, int mode);
void erase(LLIST *llist);
void *fetch(LLIST *llist, const void *key, cmp_fun *cmp, const int mode);
int update_node(LLIST *llist, const void *data, const void *newdata, const size_t size, cmp_fun cmp);
int llist_sort(LLIST *llist, int (*cmp)(const void *, const void*), void (*op)(void * data));



static inline void prefetch(const void *x) {;}
#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_replace(struct list_head *old,
				struct list_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}


static inline void list_move(struct list_head *list, struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add(list, head);
}

static inline void list_move_tail(struct list_head *list,
				  struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add_tail(list, head);
}

static inline int list_is_last(const struct list_head *list,
				const struct list_head *head)
{
	return list->next == head;
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

#define container_of(ptr, type, member) ({         \
         const typeof( ((type *)0)->member ) *__mptr = (ptr);   \
         (type *)( (char *)__mptr - offsetof(type,member) );})

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos->next, pos != (head); \
        	pos = pos->next)

#define list_for_each_entry(pos, head, member)           \
for (pos=list_entry((head)->next,typeof(*pos),member);  \
  prefetch(pos->member.next), &pos->member != (head);    \
pos = list_entry(pos->member.next, typeof(*pos), member))

#endif
