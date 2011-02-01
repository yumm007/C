#include "mem.h"

static void *re_size_add(void *addr, size_t *size) {
	char *start, *end;
	size_t offset = 0;

	offset = (unsigned)addr % (sizeof(int)) == 0 ? 0 : sizeof(int) - (unsigned)addr % (sizeof(int));
	start = (char *)addr + offset;

	offset = (unsigned)((char *)addr + *size) % (sizeof(int));
	end = (char *)addr + *size - 1 - offset;

	*size = end - start + 1;

	return (void *)start;
}

MEMLLIST *init_mem(void *addr, size_t size) {
	MEMLLIST *llist;

	llist = (MEMLLIST *) re_size_add(addr, &size);
	if (size <= sizeof(*llist))
		return NULL;

	llist->num = 1;
	llist->head.prev = llist->head.next = &llist->head;
	llist->head.size = size - sizeof(size_t)  * 2; 

	return llist;
}


static void delete_node(MEMLLIST *llist, struct free_mem_node *cur) {
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	llist->num--;
	cur->prev->size += cur->size + sizeof(size_t);
}

static void comp(MEMLLIST *llist) {
	struct free_mem_node *cur;
	int i, num;

	num = llist->num;
	for (i = 0, cur = llist->head.prev; i < num; i++, cur = cur->prev)
		if ((char *)cur == (char *)(cur->prev) + cur->prev->size + sizeof(size_t))
			delete_node(llist, cur);
}

static size_t re_size_2(size_t size) {
	size_t offset;	

	offset = (size % 4 == 0 ? 0 : sizeof(int) - size % 4);
	size = size + offset;

	if (size < sizeof(struct free_mem_node) - sizeof(size_t)) 	
		size = sizeof(struct free_mem_node) - sizeof(size_t);
	
	return size;
}

void *request_mem(MEMLLIST *llist, size_t size) {
	struct free_mem_node *cur;
	int i, notfound = 1; 
	size_t *s;
	char *ret;

	if (size == 0)
		return NULL;
	size = re_size_2(size);

	for (i = 0, cur = llist->head.prev; i < llist->num; i++, cur = cur->prev) 
		if (cur->size >= size) {
			notfound = 0;
			break;
		}
	if (notfound)
		return NULL;

	if (cur->size == size || cur->size - size - sizeof(size_t) <= sizeof(*cur)) {
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		llist->num--;
		ret = (char *)cur;
		size = cur->size;
	} else {
		cur->size = cur->size - (size + sizeof(size_t));
		ret = (char *)cur + sizeof(size_t) + cur->size;
	}

	s = (size_t *)ret;
	*s = size;

	return (void *)(s+1);
}

static void insert_free_node(MEMLLIST *llist, struct free_mem_node *pos) {
	struct free_mem_node *cur;
	ssize_t i;

	for (i = 0, cur = llist->head.next; i < llist->num; i++, cur = cur->next) 
		if ( (char *)pos > (char *)(cur->prev) && (char *)pos < (char *)cur ) 
			break;

	if (i < llist->num){
		pos->prev = cur->prev;
		pos->next = cur;
	} else {
		pos->next = &llist->head;
		pos->prev = llist->head.prev;
	}

	pos->next->prev = pos;
	pos->prev->next = pos;

	llist->num++;
	
	return;
}

void free_mem(MEMLLIST *llist, void *addr) {
	size_t size, *tmp;
	struct free_mem_node *cur;

	tmp = addr;
	size = *(--tmp);
	cur = (struct free_mem_node *)tmp;
	cur->size = size;

	insert_free_node(llist, cur);
	comp(llist);

	return;
}

void * resize_mem(MEMLLIST *llist, void *addr, size_t newsize) {
	size_t *size;
	void * newaddr;

	if (newsize == 0) {
		free_mem(llist, addr);
		return NULL;
	}

	if ( addr == NULL)
		return request_mem(llist, newsize);
	
	size = (size_t *)addr;
	size--;

	if (newsize <= *size)
		return addr;

	if ((newaddr = request_mem(llist, newsize)) == NULL)
		return NULL;

	memcpy(newaddr, addr, *size);
	free_mem(llist, addr);

	return newaddr;
}

void travel_free_mem(MEMLLIST *llist) {
	struct free_mem_node *cur;
	int i = 0;
	
	for (i = 0, cur = &llist->head; i < llist->num; cur = cur->next)
		printf("node %d, address %p, free size %d\n", ++i, cur, cur->size);

	return;
}

