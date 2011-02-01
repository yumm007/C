#include "appletree.h"

TREE *create_boxtree(void) {
	TREE * tree;

	if ((tree = malloc(sizeof (*tree))) == NULL)
		return NULL;
	
	tree->num = 0;
	tree->head = NULL;

	return tree;
}

static void __freetree(struct box *cur) {
	if (cur == NULL)
		return;
	__freetree(cur->l);
	__freetree(cur->r);
	free(cur->data);
	free(cur);
}

void freetree(TREE *tree) {
	__freetree(tree->head);
}
#if 0
static int __putbox(struct box **cur, const void *data, size_t size, cmp_box *cmp) {
	
	if (*cur == NULL) {
		void *tmp;
		if ((*cur = malloc(sizeof(**cur))) != NULL) {
			if ((tmp = malloc(size)) == NULL) {
				free(*cur);
				return -1;
			}
			(*cur)->data = tmp;
			memcpy((*cur)->data, data, size);
			(*cur)->size = size;
			(*cur)->l = (*cur)->r = NULL;
			return 0;
		} else
			return -1;	
	}
	
	if (cmp(data, (*cur)->data) <= 0)
		return __putbox(&(*cur)->l, data, size, cmp);
	return __putbox(&(*cur)->r, data, size, cmp);
}
#else
static int __putbox(struct box **cur, const void *data, size_t size, cmp_box *cmp) {
	void *tmp;

	while (*cur != NULL) {
		if (cmp(data, (*cur)->data) <= 0)
			cur = &(*cur)->l;
		else
			cur = &(*cur)->r;
	}

	if ((*cur = malloc(sizeof(**cur))) != NULL) {
		if ((tmp = malloc(size)) == NULL) {
			free(*cur);
			return -1;
		}
		(*cur)->data = tmp;
		memcpy((*cur)->data, data, size);
		(*cur)->size = size;
		(*cur)->l = (*cur)->r = NULL;
		return 0;
	} else
		return -1;	
}
	
#endif
int put_box(TREE *tree, const void* data, size_t size, cmp_box *cmp) {
	
	if ( __putbox(&tree->head, data, size, cmp) == 0) {
		tree->num++;
		return 0;
	}
	return -1;
}

static void __travel_tree(struct box *cur, op_fun *op) {
	if (cur == NULL)
		return;
	__travel_tree(cur->l, op);
	op(cur->data);
	__travel_tree(cur->r, op);
}

void travel_tree(TREE *tree, op_fun *op) {
	__travel_tree(tree->head, op);
}

static struct box *findmin(struct box *cur) {
	if (cur->l == NULL)
		return cur;
	return findmin(cur->l);
}

static struct box *findmax(struct box *cur) {
	if (cur->r == NULL)
		return cur;
	return findmax(cur->r);
}
#if 0
static int __del_box(struct box **cur, const void *data, cmp_box *cmp) {
	int ret;
	struct box *min, *tmp;
	
	if (*cur == NULL)
		return -1;
	
	if ((ret = cmp(data, (*cur)->data)) == 0) {
		tmp = *cur;
		if ((*cur)->r == NULL) {
			*cur = (*cur)->l;
		} else {
			min = findmin((*cur)->r);
			min->l = (*cur)->l;
			*cur = (*cur)->r;
		}
		free(tmp->data);
		free(tmp);
		return 0;
	} else if (ret < 0) {
		return __del_box(&(*cur)->l, data,cmp);
	} else
		return __del_box(&(*cur)->r, data,cmp);
}
#else
static int __del_box(struct box **cur, const void *data, cmp_box *cmp) {
	int ret;
	struct box *min, *tmp;
	
	while (*cur != NULL) {
		ret = cmp(data, (*cur)->data);
		if (ret == 0) {
			//find it
			tmp = *cur;
			if ((*cur)->r == NULL) {
				*cur = (*cur)->l;
			} else {
				min = findmin((*cur)->r);
				min->l = (*cur)->l;
				*cur = (*cur)->r;
			}
			free(tmp->data);
			free(tmp);
			return 0;
		} else if (ret < 0)
			cur = &(*cur)->l;
		else
			cur = &(*cur)->r;
	}

	return -1;
}

#endif
void del_box(TREE *tree, const void *data, cmp_box *cmp) {
	if (__del_box(&tree->head, data, cmp) == 0) 
		tree->num--;
}

#if 0

void *__find_box(struct box * cur, const void *data, cmp_box *cmp) {
	int ret;
	
	if (cur == NULL)
		return NULL;

	if ((ret = cmp(data, cur->data)) == 0)
		return cur->data;
	else if (ret < 0)
		return __find_box(cur->l, data, cmp);
	else
		return __find_box(cur->r, data, cmp);
}

#else

void *__find_box(struct box * cur, const void *data, cmp_box *cmp) {
	int ret;
	
	while (cur != NULL) {
		ret = cmp(data, cur->data);
		if (ret == 0)
			return cur->data;
		else if (ret < 0)
			cur = cur->l;
		else
			cur = cur->r;
	}
	
	return NULL;
}
#endif

void *find_box(TREE *tree, const void *data, cmp_box *cmp) {
	return __find_box(tree->head, data, cmp);
}








