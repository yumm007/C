#include <stdio.h>

const char *fun_name = "cd";

int fun_body(int argc, char *argv[]) {
	if (argv[1] == NULL && chdir(argv[1]) == 0)
		printf("\n");
	else if (chdir(argv[1]) != 0)
		perror("");
	return 0;
}
