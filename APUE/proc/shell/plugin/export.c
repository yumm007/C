#include <stdio.h>
#include <string.h>

const char *fun_name = "export";

int fun_body(int argc, char *argv[]) {
	char *name, *value;
	int ret = -1;
	if ((name = strtok(argv[1], "=")) != NULL && \
		(value = strtok(NULL, "\n")) != NULL) 
		ret = setenv(name, value,1);
	return ret;
}
