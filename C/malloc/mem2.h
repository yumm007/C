#ifndef HS_MEM2_H
#define HS_MEM2_H

#include <stddef.h>

#define MEMALIGN sizeof(int)


struct memnode {
	size_t size;
	struct memnode *prev, *next;
};

typedef struct {
	size_t freemem; //剩余可用的总内存
	struct memnode head;
}MEM;

MEM *init_mem(void *, size_t size);
void erase_mem(MEM *);
void *req_mem(MEM *, size_t size);
void free_mem(MEM *, void *);

#endif
