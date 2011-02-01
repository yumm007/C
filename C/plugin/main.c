#include <stdio.h>
#include <dlfcn.h>

#define FUNMAX 32
#define FUNNAME 255

typedef int (* FUN[FUNMAX])(int, int);

int get_fun_name(char *fun_name, FILE *fp) {
	int c;
	
	while ((c = fgetc(fp)) && isspace(c))
		;
	ungetc(c, fp);

	while ((c = fgetc(fp))) {
		if (c == EOF)
			return -1;
		if (c == '\n') 
			break;
		*fun_name++ = c;
	}	

	*fun_name = '\0';
	return 0;
}

int init_plugin_fun(FUN fun, void *handle) {
	FILE *plugfile;
	char fun_name[FUNNAME];
	int i;

	if ((plugfile = fopen("plugin.ini", "r")) == NULL)
		return -1;

	for (i = 0; i < FUNMAX; i++) {
		if (get_fun_name(fun_name, plugfile) == -1)
			break;
		if ((fun[i] = dlsym(handle, fun_name)) == NULL)
			return -1;;
	}

	fun[i] = NULL;

	fclose(plugfile);
	return 0;
}


int main(int argc, char *argv[])
{
	FUN fun;
	void *handle;
	int i = 0;

	if ((handle = dlopen("./pluginfun.o", RTLD_LAZY)) == NULL)
		return -1;
	
	if (init_plugin_fun(fun, handle) == -1) {
		fprintf(stderr,"%s: open plugin file faild!\n", argv[0]);
		return -1;
	}

	while (fun[i] != NULL)
		printf("60 ? 4 = %d\n", (fun[i++])(60, 4));

	dlclose(handle);
	return 0;
}
