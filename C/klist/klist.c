#include "klist.h"

LLIST *create_llist(void) {
	LLIST *llist;

	if((llist = malloc(sizeof(LLIST))) == NULL)
		return NULL;

	llist->num = 0;
	INIT_LIST_HEAD(&llist->head);
	return llist;
}

int insert_llist(LLIST *llist, const void *data, const size_t size, const int mode) {
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
		__list_add(&new->link, &llist->head, (llist->head).next);
	} else if (mode == NEXT) {
		__list_add(&new->link, (llist->head).prev, &llist->head);
	}

	llist->num++;

	return 0;
}

struct list_node * __find_node(LLIST *llist, const void *key, cmp_fun *cmp, int mode) {
	struct list_node *cur;
	
	if (mode == NEXT)
		list_for_each_entry(cur, &llist->head, link)
			if(cmp(cur->data, key) == 0) 
				break;
	else if (mode == PREV)
		list_for_each_entry(cur, &llist->head, link)
			if(cmp(cur->data, key) == 0) 
				break;
	return cur;
}

void *find_node(LLIST *llist, const void *key, cmp_fun *cmp, int mode) {
	struct list_node *cur;

	if (llist == NULL)
		return NULL; 

	cur = __find_node(llist, key, cmp, mode);
	if (&cur->link == &llist->head)
		return NULL;

	return cur->data;
}

int update_node(LLIST *llist, const void *data, const void *newdata, const size_t size, cmp_fun cmp) {
	struct list_node *cur;
	void * tmp;
	
	cur = __find_node(llist, data, cmp, NEXT);
	if (&cur->link == &llist->head)
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

	cur = __find_node(llist, key, cmp, mode);
	if (&cur->link == &llist->head)
		return -1;

	__list_del(cur->link.prev, cur->link.next);
	free(cur->data);
	free(cur);
	llist->num--;

	return 0;
}

void travel(LLIST *llist, void (*op)(void *data)) {
	struct list_node *cur;

	if (llist == NULL)
		return;

	list_for_each_entry(cur, &llist->head, link)
		op(cur->data);
}

void erase(LLIST *llist) {
	struct list_node *cur, *tmp;

	list_for_each_entry(cur, &llist->head,link)
		if (cur->link.prev != &llist->head) { 
			tmp = list_entry(cur->link.prev, struct list_node, link);
			free(tmp->data);
			free(tmp);
		}

	tmp = list_entry(cur->link.prev, struct list_node, link);
	free(tmp->data);
	free(tmp);
	free(llist);
}

void *fetch(LLIST *llist, const void *key, cmp_fun *cmp, const int mode) {
	struct list_node *cur;
	void *data;
	
	cur = __find_node(llist, key, cmp, mode);
	if (&cur->link == &llist->head)
		return NULL;

	__list_del(cur->link.prev, cur->link.next);
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

		list_for_each_entry(cur, &llist->head, link) {
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
		else if (insert_llist(llist, data, size, NEXT) == -1) 
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
	int num, i = 0;
	void * (*arr)[1], *base;
	struct list_node *cur;

	if (llist == NULL)
		return -1;
	
	num = llist->num;
	if ((base =  malloc(num * sizeof(void *))) == NULL)
		return -1;
	arr = base;

	list_for_each_entry(cur, &llist->head, link)
		(*arr)[i++] = cur->data;

	qsort(*arr, num, sizeof(void *), cmp);
	for (i = 0; i < num; i++) 
		op((*arr)[i]);		

	free(base);
	return 0;
}

