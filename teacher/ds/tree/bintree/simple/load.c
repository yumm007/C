#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <queue.h>

struct node_st {
      char data;
      struct node_st *l, *r;
};

struct node_st *glob_root = NULL;

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
      printf("%c\n", root->data);

      __draw(root->l, level + 1);
}

void draw(struct node_st *root)
{
      __draw(root, 0);
      getchar();
}

static struct node_st *__read_tree(FILE *fp)
{
      struct node_st *newnode;
      int data;

      fgetc(fp);

      data = fgetc(fp);
      if (data == ')') {
	    return NULL;
      }

      newnode = malloc(sizeof(*newnode));
      /* if error */
      newnode->data = data;
      newnode->l = __read_tree(fp);
      newnode->r = __read_tree(fp);
      fgetc(fp);

      return newnode;
}

struct node_st *load(const char *path)
{
      FILE *fp;
      struct node_st *root;

      fp = fopen(path, "r");
      /* if error */

      root = __read_tree(fp);

      fclose(fp);
      return root;
}

int main(void)
{
      int i;
      int arr[] = {7,2,8,1,5,3,6,4};

      glob_root = load("./tmp");
      draw(glob_root);

      return 0;
}
