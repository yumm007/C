#include <stdio.h>

static const char *fun_name = "echo";

static int fun_body(int argc, char *argv[]) {
	while (argc-- > 1)
		printf("%s ", *++argv);
	printf("\n");
	return 0;
}
