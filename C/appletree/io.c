#include "appletree.h"

static struct box *__read_box(FILE *fp, int *errflag) {
	void *data = NULL;;
	struct box *cur = NULL;
	size_t size;

	if (fread(&size, sizeof(size), 1, fp) < 1) {
		*errflag = 1;
		return NULL;
	}		
	if (size == 0){
		if (fread(&size, sizeof(size), 1, fp) < 1) 
			*errflag = 1;
		return NULL;
	}

	if ((cur = malloc(sizeof(*cur))) == NULL || \
			(data = malloc(size)) == NULL || \
			fread(data, size, 1, fp) < 1){
		*errflag = 1;
		free(cur);
		free(data);
		return NULL;
	}

	cur->size = size;
	cur->data = data;
	cur->l = cur->r = NULL;

	return cur;
}

static int __load_tree(struct box *cur, FILE *fp) {
	int errflag = 0;	

	if (cur == NULL)
		return 0;

	cur->l = __read_box(fp, &errflag);
	if (errflag)
		return -1;
	cur->r = __read_box(fp, &errflag);
	if (errflag)
		return -1;

	if (__load_tree(cur->l, fp) == -1 || __load_tree(cur->r, fp) == -1)
		return -1;

	return 0;
}

TREE *load_tree(void) {
	FILE *fp;
	TREE *tree;
	int n, failed = 0, errflag = 0;

	if ((fp = fopen("tree.db", "r")) == NULL)
		return NULL;

	if ((tree = create_boxtree()) == NULL)
		failed = 1;

	if (!failed && fread(&n, sizeof(n), 1, fp) < 1)
		failed = 1;

	if (!failed && (tree->head = __read_box(fp, &errflag)) == NULL) 
		failed = 1;

	if (!failed && !errflag && __load_tree(tree->head, fp) == -1)
		failed = 1;

	if (failed || errflag) {
		freetree(tree);
		free(tree);
		fclose(fp);
		return NULL;
	}

	tree->num = n;

	fclose(fp);
	return tree;
}

static int __save_box(struct box *cur, FILE *fp) {
	int zero = 0;
	
	if (cur != NULL) {
		if (fwrite(&cur->size, sizeof(cur->size), 1, fp) < 1 || \
			fwrite(cur->data, cur->size, 1, fp) < 1 )
			return -1;
	} else {
		if (fwrite(&zero, sizeof(zero), 1, fp) < 1 || \
			fwrite(&zero, sizeof(zero), 1, fp) < 1)
			return -1;
	}
	
	return 0;
}
 
static int __save_tree(struct box *cur, FILE *fp) {

	if (cur == NULL) 
		return 0;

	if (__save_box(cur->l, fp) == -1 || __save_box(cur->r, fp) == -1)
		return -1;

	if (__save_tree(cur->l, fp) == -1 || __save_tree(cur->r, fp) == -1)
		return -1;

	return 0;
}

int save_tree(TREE *tree) {
	FILE *fp;

	if ((fp = fopen("tree.db", "w")) == NULL) 
		return -1;

	if (fwrite(&tree->num, sizeof(tree->num), 1, fp) < 1 || \
		fwrite(&tree->head->size, sizeof(tree->head->size), 1, fp) < 1 || \
		fwrite(tree->head->data, tree->head->size, 1, fp) < 1) {
			fclose(fp);
			return -1;
	}

	if (__save_tree(tree->head, fp) == -1) {
		fclose(fp);
		return -1;
	}

	fclose(fp);
	return 0;
}

#if 0
static __save_tree(struct box *cur, FILE *fp) {
	
	if (cur == NULL)
		return;

	fwrite(&cur->size, sizeof(cur->size), 1,fp);
	fwrite(cur->data, cur->size, 1, fp);

	__save_tree(cur->l, fp);
	__save_tree(cur->r, fp);

}

int save_tree(TREE *tree) {
	FILE *fp;

	if ((fp = fopen("tree.db", "w")) == NULL) 
		return -1;

	if (fwrite(&tree->num, sizeof(tree->num), 1, fp) < 1) {
		fclose(fp);
		return -1;
	}

	__save_tree(tree->head, fp);

	return 0;
}


static int __load_tree(TREE *tree, FILE *fp, cmp_box *cmp, size_t n) {
	size_t size;
	void *tmp;
	
	while (n--) {
		fread(&size, sizeof(size), 1, fp);
		if ((tmp = malloc(size)) == NULL)
			return -1;
		fread(tmp, size, 1, fp);
		if (put_box(tree, tmp, size, cmp) == -1)
			return -1;
		free(tmp);
	}

	return 0;
}

TREE *load_tree(cmp_box *cmp) {

	FILE *fp;
	TREE *tree;
	int n;

	if ((fp = fopen("tree.db", "r")) == NULL)
		return NULL;

	if ((tree = create_boxtree()) == NULL)
		return NULL;
	
	fread(&n, sizeof(tree->num), 1, fp);

	if (__load_tree(tree, fp, cmp, n) == -1){
		freetree(tree);
		fclose(fp);
		return NULL;
	}

	fclose(fp);
	return tree;
}

#endif
