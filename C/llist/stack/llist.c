#include "llist.h"

LLIST *create_llist(void) {
	LLIST *llist;

	if((llist = malloc(sizeof(LLIST))) == NULL)
		return NULL;

	llist->num = 0;
	(llist->head).data = NULL;
	(llist->head).size = 0;
	(llist->head).prev = (llist->head).next = &(llist->head);

	return llist;
}

int insert_llist(LLIST *llist, const void *data, size_t size, int mode) {
	struct list_node *new;
	void *tmp;

	if ((new = malloc(sizeof(struct list_node))) == NULL)
		return -1;
	if ((tmp = malloc(size)) == NULL) {
		free(new);
		return -1;
	}

	memcpy(tmp, data, size);	
	new->size = size;
	new->data = tmp;

	if (mode == PREV) {
		new->next = (llist->head).next;
		new->prev = &(llist->head);
	} else if (mode == NEXT) {
		new->next = &(llist->head);
		new->prev = (llist->head).prev;
	}

	new->next->prev = new;
	new->prev->next = new;	

	llist->num++;

	return 0;
}

struct list_node * __find_node(LLIST *llist, const void *key, cmp_fun *cmp, int mode) {
	struct list_node *cur;
	
	if (mode == PREV){
		for(cur = (llist->head).next; cur != &(llist->head); cur = cur->next) 
			if(cmp(cur->data, key) == 0) 
				break;
	} else if (mode == NEXT){
		for(cur = (llist->head).prev; cur != &(llist->head); cur = cur->prev)
			if(cmp(cur->data, key) == 0) 
				break;
	}
	return cur;
}

void *find_node(LLIST *llist, const void *key, cmp_fun *cmp, int mode) {
	struct list_node *cur;

	if (llist == NULL)
		return NULL; 

	if ((cur = __find_node(llist, key, cmp, mode)) == &(llist->head))
		return NULL;

	return cur->data;
}

int update_node(LLIST *llist, const void *data, const void *newdata, size_t size, cmp_fun cmp) {
	struct list_node *cur;
	void * tmp;
	
	if ((cur = __find_node(llist, data, cmp, NEXT)) == &(llist->head))
		return -1;
	
	if ((tmp = malloc(size)) == NULL)
		return -1;

	memcpy(tmp, newdata, size);
	cur->size = size;
	free(cur->data);
	cur->data = tmp;

	return 0;
}

int delete_node(LLIST *llist, const void *key, cmp_fun *cmp, int mode) {
	struct list_node *cur;

	if ((cur = __find_node(llist, key, cmp, mode)) == &(llist->head))
		return -1;

	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	free(cur->data);
	free(cur);
	llist->num--;

	return 0;
}

void travel(LLIST *llist, void (*op)(void *data)) {
	struct list_node *cur;

	if (llist == NULL)
		return;

	for (cur = (llist->head).next; cur != &(llist->head); cur = cur->next )
		op(cur->data);
}

void erase(LLIST *llist) {
	struct list_node *cur;

	for (cur = (llist->head).next->next; cur != &(llist->head); cur = cur->next )
	free(cur->prev);
	free(llist);
}

void *fetch(LLIST *llist, const void *key, cmp_fun *cmp, int mode) {
	struct list_node *cur;
	void *data;
	
	if ((cur = __find_node(llist, key, cmp, mode)) == &(llist->head))
		return NULL;

	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;

	data = cur->data;
	free(cur);
	llist->num--;

	return data;
}

int save_llist(const LLIST *llist, const char *filepath) {
	FILE *fp;
	struct list_node *cur;

	if ((fp = fopen(filepath,"w")) == NULL)
		return -1;
	if (fwrite(&llist->num, sizeof(llist->num), 1, fp) < 1)
		return -1;

	for (cur = (llist->head).next; cur != &(llist->head); cur = cur->next) {
		fwrite(&cur->size, sizeof(ssize_t),1, fp);
		fwrite(cur->data, cur->size, 1, fp);
	}

	return 0;
}

LLIST *load_llist(const char *filepath){
	FILE *fp;
	LLIST *llist;
	int i, faild = 0;
	size_t size = 0, num = 0;
	void * data = NULL;

	if ((fp = fopen(filepath, "r")) == NULL)
		return NULL;
	if ((llist = create_llist()) == NULL) {
		fclose(fp);
		return NULL;
	}

	if (fread(&num, sizeof(llist->num), 1, fp) < 1) {
		free(llist);
		fclose(fp);
		return NULL;
	}

	for (i = 0; !faild && i < num; i++) {
		if (fread(&size, sizeof(size_t) ,1, fp) < 1)
			faild = 1;
		else if ((data = realloc(data, size)) == NULL) 
			faild = 1;
		else if (fread(data, size, 1, fp) < 1)
			faild = 1;
		else if (insert_llist(llist, data, size, PREV) == -1) 
			faild = 1;
	}

	if (faild) {
		fclose(fp);
		free(llist);
		free(data);
		return NULL;
	}
	
	free(data);
	fclose(fp);
	return llist;
}

int llist_sort(LLIST *llist, int (*cmp)(const void *, const void*), void (*op)(void * data)) {
	int num, i;
	void * (*arr)[1], *base;
	struct list_node *cur;

	if (llist == NULL)
		return -1;
	
	num = llist->num;
	if ((base =  malloc(num * sizeof(void *))) == NULL)
		return -1;
	arr = base;

	for (i = 0, cur = llist->head.next; i < num; i++, cur = cur->next) 
		(*arr)[i] = cur->data;

	qsort(*arr, num, sizeof(void *), cmp);
	for (i = 0; i < num; i++) 
		op((*arr)[i]);		

	free(base);
	return 0;
}

