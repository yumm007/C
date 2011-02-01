#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMESIZE 32
struct score {
      int id;
      char name[NAMESIZE];
      int math;
};

struct node_st {
      struct score data;
      struct node_st *l, *r;
};

void print_s(struct score *data)
{
      printf("%d, %s, %d\n", data->id, data->name, data->math);
}

int insert(struct node_st **root, struct score *data)
{
      if (*root == NULL) {
	    *root = malloc(sizeof(**root));
	    if (*root == NULL) {
		  return -1;
	    }

	    memcpy(&(*root)->data, data, sizeof(*data));
	    (*root)->l = (*root)->r = NULL;
	    return 0;
      }

      if (data->id <= (*root)->data.id)
	    	return insert(&(*root)->l, data);
      return insert(&(*root)->r, data);
}
#if 0
void travel(struct node_st *root)
{
      if (root == NULL) {
	    return;
      }

      print_s(&root->data);
      travel(root->l);
      travel(root->r);
}

#else

void travel(struct node_st *root)
{
      if (root == NULL) {
	    return;
      }

      travel(root->l);
      travel(root->r);
      print_s(&root->data);
}

#endif

int cmp_id(const void *key, const void *data2) {
	return (*((int *)key) - ((struct score *)data2)->id);
}

void *find(struct node_st *root, const void *key, int (*cmp)(const void *, const void *))
{
		static int notfound = 1;	
		int ret;

      if (root == NULL) {
	    return NULL;
      }

		if ((ret = cmp(key, &root->data)) == 0) {
			notfound = 0;
			return &root->data;
		} else if (notfound && ret < 0)
			return find(root->l, key, cmp);
      else if (notfound && ret > 0)
      	return find(root->r, key, cmp);
}


int main(void)
{
      struct score tmp;
      int i;
      int arr[] = {5,3,4,2,1,8,7,6,9};
      struct node_st *root = NULL;

      for (i = 0; i < sizeof(arr) / sizeof(*arr); i++) {
	    tmp.id = arr[i];
	    tmp.math = 100 - arr[i];
	    snprintf(tmp.name, NAMESIZE, "stu%d", arr[i]);

	    insert(&root, &tmp);
      }

      travel(root);
		printf("find ID 1: \n");
		i = 1;
		print_s((struct score *)find(root, &i, cmp_id));

      return 0;
}
