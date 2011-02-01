#include "stack.h"

STACK *create_stack(void) {
	return create_llist();
}

void erase_stack(STACK *ptr) {
	erase((LLIST *)ptr);
}

int push_stack(STACK *ptr, const void *data, size_t size) {
	return insert_llist((LLIST *)ptr, data, size, PREV);
}

static int null_cmp(const void *data1, const void *data2) {
	return 0;
}

void* pop_stack(STACK *ptr) {
	return fetch((LLIST *)ptr, NULL, null_cmp, PREV);
}

