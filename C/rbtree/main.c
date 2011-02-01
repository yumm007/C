#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum {RED, BLACK};
struct node_st {
      int data;
      int color;
      struct node_st *p, *l, *r;
} nil = {0, BLACK, &nil, &nil, &nil};

static void __draw(struct node_st *tree, int level)
{
      int i;

      if (tree == &nil) {
	    return;
      }

      __draw(tree->r, level + 1);

      for (i = 0; i < level; i++) {
	    printf("      ");
      }
      printf("%d ", tree->data);
      if (tree->color == RED) {
	    printf("R\n");
      } else {
	    printf("B\n");
      }

      __draw(tree->l, level + 1);
}

void draw(struct node_st *tree)
{
      __draw(tree, 0);
      getchar();
}

static struct node_st *find_p(struct node_st *tree, int data)
{
      if (tree == &nil) {
	    return &nil;
      }

      if (data < tree->data) {
	    if (tree->l == &nil) {
		  return tree;
	    }
	    return find_p(tree->l, data);
      }

      if (tree->r == &nil) {
	    return tree;
      }
      return find_p(tree->r, data);
}

static void turn_left(struct node_st *node, struct node_st **tree)
{
      struct node_st *right = node->r;

      if (node == *tree) {
	    *tree = right;
      } else {
	    if (node == node->p->l) {
		  node->p->l = right;
	    } else {
		  node->p->r = right;
	    }
      }
      right->p = node->p;
      node->p = right;

      node->r = right->l;
      right->l = node;

      if (node->r != &nil) {
	    node->r->p = node;
      }
}

/* fix me */
static void turn_right(struct node_st *node, struct node_st **tree)
{
      struct node_st *left = node->l;

      if (node == *tree) {
	    *tree = left;
      } else {
	    if (node == node->p->l) {
		  node->p->l = left;
	    } else {
		  node->p->r = left;
	    }
      }
      left->p = node->p;
      node->p = left;

      node->l = left->r;
      left->r = node;

      if (node->l != &nil) {
	    node->l->p = node;
      }
}

static void insert_balance(struct node_st **tree, struct node_st *node)
{
      while (node != *tree && node->p->color == RED) {
	    if (node->p == node->p->p->l) {
		  /* case 1 */
		  if (node->p->p->r->color == RED) {
			node->p->color = node->p->p->r->color = BLACK;
			node->p->p->color = RED;
			node = node->p->p;
			continue;
		  }

		  /* case 2 */
		  if (node == node->p->r) {
			node = node->p;
			turn_left(node, tree);
		  }

		  /* case 3 */
		  node->p->color = BLACK;
		  node->p->p->color = RED;
		  turn_right(node->p->p, tree);
	    } else {
		  /* case 1 */
		  if (node->p->p->l->color == RED) {
			node->p->color = node->p->p->l->color = BLACK;
			node->p->p->color = RED;
			node = node->p->p;
			continue;
		  }

		  /* case 2 */
		  if (node == node->p->l) {
			node = node->p;
			turn_right(node, tree);
		  }

		  /* case 3 */
		  node->p->color = BLACK;
		  node->p->p->color = RED;
		  turn_left(node->p->p, tree);
	    }

	    node = *tree;
      }

      (*tree)->color = BLACK;
}

int insert(struct node_st **tree, int data)
{
      struct node_st *newnode, *p;

      newnode = malloc(sizeof(*newnode));
      if (newnode == NULL) {
	    return -1;
      }
      newnode->data = data;
      newnode->color = RED;
      newnode->l = newnode->r = &nil;

      p = find_p(*tree, data);
      if (p == &nil) {
	    newnode->p = &nil;
	    newnode->color = BLACK;
	    *tree = newnode;
	    return 0;
      }

      if (data <= p->data) {
	    p->l = newnode;
      } else {
	    p->r = newnode;
      }
      newnode->p = p;

      insert_balance(tree, newnode);
      return 0;
}

static void delet_balance(struct node_st **tree, struct node_st *node)
{
      while (node != *tree && node->color != RED) {
	    if (node == node->p->l) {
		  /* case 1 */
		  if (node->p->r->color == RED) {
			node->p->r->color = BLACK;
			node->p->color = RED;
			turn_left(node->p, tree);
		  }

		  /* case 2 */
		  if (node->p->r->l->color == BLACK && node->p->r->r->color == BLACK) {
			node->p->r->color = RED;
			node = node->p;
			continue;
		  }

		  /* case 3 */
		  if (node->p->r->r->color == BLACK) {
			node->p->r->color = node->p->r->l->color;
			node->p->r->l->color = BLACK;
			turn_right(node->p->r, tree);
		  }

		  /* case 4 */
		  node->p->r->color = node->p->color;
		  node->p->color = BLACK;
		  node->p->r->r->color = BLACK;
		  turn_left(node->p, tree);
		  node = *tree;
	    } else {
		  /* case 1 */
		  if (node->p->l->color == RED) {
			node->p->l->color = BLACK;
			node->p->color = RED;
			turn_right(node->p, tree);
		  }

		  /* case 2 */
		  if (node->p->l->l->color == BLACK && node->p->l->r->color == BLACK) {
			node->p->l->color = RED;
			node = node->p;
			continue;
		  }

		  /* case 3 */
		  if (node->p->l->l->color == BLACK) {
			node->p->l->color = node->p->l->r->color;
			node->p->l->r->color = BLACK;
			turn_left(node->p->l, tree);
		  }

		  /* case 4 */
		  node->p->l->color = node->p->color;
		  node->p->color = BLACK;
		  node->p->l->l->color = BLACK;
		  turn_right(node->p, tree);
		  node = *tree;
	    }
      }

      node->color = BLACK;
}

void delete(struct node_st **tree, int data)
{
      struct node_st *cur, **root = tree;

      cur = *root;
      while (cur != &nil) {
	    if (data == cur->data) {
		  break;
	    }
	    if (data < cur->data) {
		  root = &cur->l;
		  cur = cur->l;
	    } else {
		  root = &cur->r;
                  cur = cur->r;
	    }
      }
      if (cur == &nil) {
	    return;
      }

      if (cur->r == &nil) {
	    *root = cur->l;
	    cur->l->p = cur->p;
      } else {
	    if (cur->l != &nil) {
		  for (root = &cur->r; (*root)->l != &nil; root = &(*root)->l)
			;
		  cur->data = (*root)->data;
		  cur = *root;
	    }
	    *root = cur->r;
	    cur->r->p = cur->p;
      }

      if (cur->color == BLACK) {
	    delet_balance(tree, *root);
      }

      free(cur);
}

int main(void)
{
      int arr[] = {1,2,33,3,4,5,6,7,8,9,11,22,};
      int i;
      struct node_st *tree = &nil;

      for (i = 0; i < sizeof(arr) / sizeof(*arr); i++) {
	    insert(&tree, arr[i]);
      }

      draw(tree);
      delete(&tree, 11);
      draw(tree);

      return 0;
}
