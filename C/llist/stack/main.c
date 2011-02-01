#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main(void)
{
	STACK *stack;
	int i, *ret;

	if ((stack = create_stack()) == NULL) {
		printf("create stack error!\n");
		return -1;
	}

	for (i = 0 ; i < 10; i++)
		if (push_stack(stack, &i, sizeof(i)) == -1) {
			printf("push %d error!\n", i);
			break;
		}

	for (i = 0 ; i < 12; i++) {
		if ((ret = (int *)(pop_stack(stack))) == NULL) {
			printf("\npop %d error!\n", i);
			break;
		}
		printf("%d, ", *ret);
	}
	printf("\n");

	erase_stack(stack);

	return 0;
}
