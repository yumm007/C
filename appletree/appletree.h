#ifndef _HS_APPLETREE_H
#define _HS_APPLETREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct box {
	size_t size;
	void *data;
	struct box *l, *r;
};

typedef struct {
	int num;
	struct box *head;
}TREE;

typedef int cmp_box(const void *box1, const void *box2);
typedef void op_fun(const void *);

TREE *create_boxtree(void);
void freetreee(TREE *);
int put_box(TREE *, const void *, size_t, cmp_box *); //插入节点
void del_box(TREE *, const void *, cmp_box *); //删除节点
//void *fetch_box(TREE *, struct box, cmp_box *); //fetch节点
void *find_box(TREE *, const void*, cmp_box *);
void travel_tree(TREE *, op_fun *);

#endif
