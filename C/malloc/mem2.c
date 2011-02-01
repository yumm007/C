#include "mem2.h"

#define offset_fun(num) ((num) % MEMALIGN == 0 ? 0: MEMALIGN - (num) % MEMALIGN )

static void *align_mem(void *ptr, size_t *size) {
	size_t offset;
	
	offset = offset_fun((unsigned long)ptr);
	ptr = (void *)((char *)ptr + offset);

	*size -= offset;
	offset = offset_fun(*size);
	*size -= offset;
	
	return ptr;
}

MEM *init_mem(void *ptr, size_t size) {
	MEM *head;
	struct memnode *first;

	//地址和大小对齐
	ptr = align_mem(ptr, &size);
	
	head = (MEM *)ptr;
	head->head.prev = head->head.next =(struct memnode *)(head + 1);

	first = (struct memnode *)(head + 1);
	first->prev = first->next = &head->head;

	//将实际大小转换成可用大小. 可用大小还需要对齐吗?
	size = size - sizeof(*head) - sizeof(size_t);
	head->freemem = first->size = size;
	
	return head;
}

void erase_mem(MEM *ptr) {
	return;
}

size_t align_size(size_t size) {
	if (size < sizeof(struct memnode))
		return sizeof(struct memnode);
	return size + offset_fun(size);
}

/*
void *req_mem(MEM *ptr, size_t size) {
	struct memnode *cur;
	size_t *nodesize;

	size = align_size(size); //符合对齐和最小容量(一个memnode)

	//fetch函数找到一个大小相等或者大于size的node
	//没有的话则从一个较大的node中分离一个node并返回
	if ((cur = fetch_node(ptr, size)) == NULL)
		return split_node(ptr, size);
	
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	
	return (void *)(&cur->prev); // 返回size后面的值
}

void free_mem(MEM *ptr, void *node) {
	struct memnode *cur;

	cur = (struct memnode *)node;

	//插入freenode链表
	insert_node(ptr, cur);

	//合并连续空闲内存
	comp(ptr);
}
*/
