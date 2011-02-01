#include <stdio.h>
#include "darr.h"

#define MAXNAME 20

struct scope {
	int id;
	char name[MAXNAME];
	int scope;
};

int cmp_id(void *data, void *key) {
	return ((struct scope *)data)->id - *(int *)key;
}

void print_node(void *data) {
	struct scope *node;
	node = (struct scope *)data;
	printf("ID:%d, name:%s, scope:%d\n",node->id,node->name,node->scope);
}

int main(void)
{
	DARR *darr, *tmparr;
	struct scope tmp;
	int i;
	
	if ((darr = load_darr("db.db")) == NULL){
		printf("load db error.\n");
		if ((darr = create_darr(sizeof(struct scope))) == NULL) {
			printf("create date error.\n");
			return -1;
		}
	}
	
	for (i = 1; i <= 20; i++) {
		tmp.id = i;
		tmp.scope = 100 -i;
		snprintf(tmp.name, MAXNAME, "stu%d", i);
		if (append_darr(darr, &tmp) == -1)
			break;
	}

	i = 4;
	print_node(fetch(darr,&i,cmp_id));
#if 0
	for (i = 2; i < 13; i++)
	  delete_node(darr,&i,cmp_id);
	travel(darr,print_node);
	i = 15;
	print_node(fetch(darr,&i,cmp_id));
	travel(darr,print_node);
	travel(darr,print_node);
	i = 10;
	tmparr = fetch(darr, &i, cmp_id);
	printf("----------------------\n");
	print_node(tmparr);
	printf("----------------------\n");
	inserd_node(darr,tmparr,1);
	travel(darr,print_node);
	//save_darr(darr, "db.db");
	erase(darr);
	//printf("after travel\n");
	
#endif
	return 0;
}


