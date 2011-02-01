#include <stdio.h>
#include "appletree.h"

int cmp_num(const void *box1, const void *box2) {
	return *(int *)box1 - *(int *)box2;
}

void print_num(const void *box) {
	printf("%d\n", *(int *)box);
}

int main(void)
{
	int i;
	int num[] = {60,6,89,45,100,67,12,89,37,30,94,73,15,7,45,23};
	TREE *tree;
	void *tmp;
#if 0
	if ((tree = create_boxtree()) == NULL) {
		printf("create boxtree faild!\n");
		return -1;
	}

	for (i = 0; i < sizeof(num) / sizeof(*num); i++){
		if (put_box(tree, &num[i], sizeof(*num), cmp_num) == -1) {
			printf("insert box faild.\n");
			break;
		}
	}
#else
	if ((tree = load_tree()) == NULL) {
		printf("load boxtree faild!\n");
		return -1;
	}
#endif	

//	draw_tree(tree);

	printf("------------------------\n");
	balance(tree);
	draw_tree(tree);
	
//	travel_tree(tree, print_num);
	//put_box(tree, &i, sizeof(i), cmp_num);
	//printf("after insert %d\n", i);
	//travel_tree(tree, print_num);

#if 0
	i = 37;
	//del_box(tree, &i, cmp_num);
	printf("------------after del %d.\n", i);
	travel_tree(tree, print_num);
	i = 60;
	//del_box(tree, &i, cmp_num);
	i = 89;
	printf("---------find %d.\n", i);
	if ((tmp = find_box(tree, &i, cmp_num))!= NULL)
		print_num(tmp);
	else
		printf("not found.\n");

	printf("node number %d\n", tree->num);

#endif

	if (save_tree(tree) == -1) {
		printf("save to file error.\n");
	}
	freetree(tree);
	return 0;
}

