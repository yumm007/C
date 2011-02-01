#include <stdio.h>
#include <string.h>

//使用双向链表存储子节点

struct trie_node;

struct trie_inode {
	void *key;
	struct trie_node *up;
	struct trie_inode *prev;
	struct trie_inode *next;
};

struct trie_node {
	struct trie_inode inode;
	size_t size;
	void *data;
};


struct trie_node *create_trie(void);
void free_trie(struct trie_node *);
//int insert_trie_node()
