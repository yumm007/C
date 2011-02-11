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

int insert(struct node_st **root, int data)
{
      struct node_st *newnode;
      newnode = malloc(sizeof(*newnode));
      if (newnode == NULL) {
	    return -1;
      }
      data += '0';
      newnode->data = data;
      newnode->l = newnode->r = NULL;

      while (*root != NULL) {
	    if (data <= (*root)->data) {
		  root = &(*root)->l;
	    } else {
		  root = &(*root)->r;
	    }
      }

      *root = newnode;
      return 0;
}

static int __write_tree(FILE *fp, struct node_st *root)
{
      fputc('(', fp);		/* if error */

      if (root == NULL) {
	    fputc(')', fp);	/* if error */
	    return 0;
      }

      fputc(root->data, fp);	/* if error */

      __write_tree(fp, root->l); /* if error */
      __write_tree(fp, root->r); /* if error */

      fputc(')', fp);	/* if error */
      return 0;
}

int save(struct node_st *root, const char *path)
{
      FILE *fp;

      fp = fopen(path, "w");
      if (fp == NULL) {
	    perror(path);	/* fix me */
	    return -1;
      }

      __write_tree(fp, root);
      /* if error */

      fclose(fp);
}

int main(void)
{
      int i;
      int arr[] = {7,2,8,1,5,3,6,4};

      for (i = 0; i < sizeof(arr) / sizeof(*arr); i++) {
	    insert(&glob_root, arr[i]);
      }

      draw(glob_root);
      save(glob_root, "./tmp");

      return 0;
}
