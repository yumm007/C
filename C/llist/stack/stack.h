#ifndef _HS_STACK_H
#define _HS_STACK_H

#include "llist.h"

typedef LLIST STACK;

STACK *create_stack(void);

void erase_stack(STACK *ptr);

int push_stack(STACK *ptr, const void *data, size_t size);

void* pop_stack(STACK *ptr);

#endif
