#include "myshell.h"

int reg_fun_arr(const char *filepath, struct buildin_fun *cur) {
	void *handler, *tmp;
	
	if ((handler = dlopen(filepath, RTLD_LAZY)) == NULL) 
		return -1;

	if ((tmp = dlsym(handler, "fun_name")) == NULL) {
		dlclose(handler);
		return -1;
	}
	cur->fun_name = *(char **)tmp;
	if ((cur->fun_body = dlsym(handler, "fun_body")) == NULL) {
		dlclose(handler);
		return -1;
	}
	
	cur->handler = handler;
	return 0;
}

int load_buildin_fun(struct buildin_fun **fun_arr) {
	glob_t fun_glob;
	int i = 0, j = 0;
	
	if (glob(PLUGIN_PATH, GLOB_NOSORT, NULL, &fun_glob) != 0)
		return j;

	if ((*fun_arr = malloc(sizeof(**fun_arr) * (fun_glob.gl_pathc + 1))) != NULL) 
		while (fun_glob.gl_pathv[i] != NULL)
			if (reg_fun_arr(fun_glob.gl_pathv[i++], *fun_arr + j) == 0) 
				j++;			

	globfree(&fun_glob);
	(*fun_arr + j)->fun_name = NULL;
	(*fun_arr + j)->fun_body = NULL;

	return j;
}

void free_buildin_fun(struct buildin_fun *cur) {
	free(cur);
}

fun_fmt is_buildin_fun(const struct buildin_fun *fun_arr, const char *cmd) {
	int i;
	if (cmd == NULL || fun_arr == NULL)
		return NULL;
	for (i = 0; (fun_arr + i)->fun_name != NULL; i++)
		if (strcmp(cmd, (fun_arr + i)->fun_name) == 0)
			return (fun_arr + i)->fun_body;
	return NULL;
}

