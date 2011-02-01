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

struct node_st *glob_root = NULL;

void print_s(struct score *data)
{
      printf("%d, %s, %d\n", data->id, data->name, data->math);
}

static void __draw(struct node_st *root, int level)
{
      int i;

      if (root == NULL) {
	    return;
      }

      __draw(root->r, level + 1);

      for (i = 0; i < level; i++) {
	    printf("     ");
      }
      //print_s(&root->data);
      printf("%d\n", root->data.id);

      __draw(root->l, level + 1);
}

void draw(struct node_st *root)
{
      __draw(root, 0);
      getchar();
}

int get_num(struct node_st *root)
{
      if (root == NULL) {
	    return 0;
      }

      return get_num(root->l) + 1 + get_num(root->r);
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

      if (data->id <= (*root)->data.id) {
	    return insert(&(*root)->l, data);
      }
      return insert(&(*root)->r, data);
}

void travel(struct node_st *root)
{
      if (root == NULL) {
	    return;
      }

      travel(root->l);
      print_s(&root->data);
      travel(root->r);
}

static struct node_st *find_max(struct node_st *root)
{
      struct node_st *cur;

      for (cur = root; cur->r != NULL; cur = cur->r)
	    ;

      return cur;
}

static struct node_st *find_min(struct node_st *root)
{
      struct node_st *cur;

      for (cur = root; cur->l != NULL; cur = cur->l)
	    ;

      return cur;
}

void delete(struct node_st **root, int id)
{
      struct node_st *cur;

      if (*root == NULL) {
	    return;
      }

      if (id == (*root)->data.id) {
	    cur = *root;

	    if (cur->l == NULL) {
		  *root = cur->r;
		  free(cur);
		  return;
	    }

	    *root = cur->l;
	    find_max(cur->l)->r = cur->r;
	    free(cur);
      } else if (id < (*root)->data.id) {
	    delete(&(*root)->l, id);
      } else {
	    delete(&(*root)->r, id);
      }
}

static void turn_left(struct node_st **root)
{
      struct node_st *cur = *root;

      *root = cur->r;
      cur->r = NULL;

      find_min(*root)->l = cur;

      draw(glob_root);
}

static void turn_right(struct node_st **root)
{
      struct node_st *cur = *root;

      *root = cur->l;
      cur->l = NULL;

      find_max(*root)->r = cur;

      draw(glob_root);
}

void balance(struct node_st **root)
{
      int sub;

      if (*root == NULL) {
	    return;
      }

      while (1) {
	    sub = get_num((*root)->l) - get_num((*root)->r);
	    if (sub >= -1 && sub <= 1) {
		  break;
	    }
	    if (sub > 1) {
		  turn_right(root);
	    } else {
		  turn_left(root);
	    }
      }

      balance(&(*root)->l);
      balance(&(*root)->r);
}

int main(void)
{
      struct score tmp;
      int i;
      int arr[] = {1,2,3,33,4,5,11,55,77,6,7,8,9,};

      for (i = 0; i < sizeof(arr) / sizeof(*arr); i++) {
	    tmp.id = arr[i];
	    tmp.math = 100 - arr[i];
	    snprintf(tmp.name, NAMESIZE, "stu%d", arr[i]);

	    insert(&glob_root, &tmp);
      }

      draw(glob_root);
      //delete(&glob_root, 5);
      balance(&glob_root);
      draw(glob_root);

      //travel(glob_root);

      return 0;
}
