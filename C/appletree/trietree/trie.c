#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFSIZE 1024

struct node_st {
      int times;
      struct node_st *arr[26];
};

int getword(FILE *fp, char *buf, int size)
{
      int c, i;

      while (1) {
	    c = fgetc(fp);
	    if (isalpha(c) || c == EOF) {
		  break;
	    }
      }

      if (c == EOF) {
	    buf[0] = '\0';
	    return 0;
      }

      if (isupper(c)) {
	    buf[0] = c - 'A' + 'a';
      } else {
	    buf[0] = c;
      }
      for (i = 1; i < size - 1; i++) {
	    c = fgetc(fp);
	    if (!isalpha(c)) {
		  break;
	    }

	    if (isupper(c)) {
		  buf[i] = c - 'A' + 'a';
	    } else {
		  buf[i] = c;
	    }
      }

      buf[i] = '\0';
      return i;
}

static struct node_st *creat_node(void)
{
      struct node_st *newnode;
      int i;

      newnode = malloc(sizeof(*newnode));
      if (newnode == NULL) {
	    return NULL;
      }

      for (i = 0; i < 26; i++) {
	    newnode->arr[i] = NULL;
      }
      newnode->times = 0;

      return newnode;
}

int insert(struct node_st **root, const char *word)
{
      if (*root == NULL) {
	    *root = creat_node();
	    if (*root == NULL) {
		  return -1;
	    }
      }

      if (*word == '\0') {
	    (*root)->times++;
	    return 0;
      }

      return insert(&(*root)->arr[*word - 'a'], word + 1);
}

void __travel(struct node_st *root, char *buf, int ind)
{
   int i;
	int c = 0;

      if (root == NULL) {
	    return;
      }

      if (root->times != 0) {
	    printf("%s %d\n", buf, root->times);
      }

      for (i = 0; i < 26; i++) {
	    buf[ind] = 'a' + i;
	    buf[ind + 1] = '\0';
	    __travel(root->arr[i], buf, ind + 1);
      }
}

void travel(struct node_st *root)
{
      char buf[BUFSIZE];

      __travel(root, buf, 0);
}

void destroy(struct node_st * cur)
{
	int i;

   if (cur == NULL) 
	   return;

   for (i = 0; i < 26; i++) 
	   destroy(cur->arr[i]);
	
 	cur->times = 0;
	free(cur);

	return;
}

void delete(struct node_st * trie, char *key)
{
	if(trie == NULL)
		return;

   if (*key == '\0') {
	   trie->times = 0;
	   return ;
   }

   return delete(trie->arr[*key - 'a'], key + 1);
}

int main(int argc, char **argv)
{
      FILE *fp;
      struct node_st *trie = NULL;
      char buf[BUFSIZE];
      int ret;

      if (argc != 2) {
	    fprintf(stderr, "argment...\n");
	    return 1;
      }

      fp = fopen(argv[1], "r");
      if (fp == NULL) {
	    perror(argv[1]);
	    return 1;
      }

      while (1) {
	    ret = getword(fp, buf, BUFSIZE);
	    if (ret == 0) {
		  break;
	    }

	    insert(&trie, buf);
      }

	travel(trie);
	delete(trie, "tcp");

  // destroy(trie);
	printf("\n-----------------------------------\n");

	travel(trie);
   fclose(fp);
   
	return 0;
}
