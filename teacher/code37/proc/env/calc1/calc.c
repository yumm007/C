#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <glob.h>

#define MAXOPS 512

#define PLUGIN_PAT "plugins/*.plugin"

typedef int op_func_t(int, int);
typedef struct {
	void *dlhandle;
	op_func_t *funcp;
	int sym;
	char *desc;
} op_t;

static op_func_t add, sub, mul, idiv, mod;

static op_t op_arr[MAXOPS];
static int nrops=0;

#define OP(i) (op_arr[i].funcp)
#define SYM(i) (op_arr[i].sym)

int
main()
{
	int ret;
	int i1, i2, i;
	glob_t glob_res;
	void *handle;

	ret = glob(PLUGIN_PAT, GLOB_NOSORT, NULL, &glob_res);
	if (ret!=0) {
		fprintf(stderr, "glob() error.\n");
		exit(1);
	}

	for (i=0;i<glob_res.gl_pathc;++i) {
		handle = dlopen(glob_res.gl_pathv[i], RTLD_LAZY);
		if (handle==NULL) {
			continue;
		}
		op_arr[nrops].dlhandle = handle;
		op_arr[nrops].funcp = dlsym(handle, "op_func");
		op_arr[nrops].sym = *(char*)dlsym(handle, "op_sym");
		op_arr[nrops].desc = *(char**)dlsym(handle, "op_desc");
		/* FIXME: Handle unexpected plugins */
		fprintf(stderr, "Info: Plugin <%s> installed.\n", op_arr[nrops].desc);
		nrops++;
	}
	globfree(&glob_res);

	for (i1=0;i1<nrops;++i1) {
	for (i2=0;i2<nrops;++i2) {
		if (OP(i2)(OP(i1)(2, 4), 6)==2) {
			printf("(2%c4)%c6=2\n", SYM(i1), SYM(i2));
		}
	}
	}

	for (i=0;i<nrops;++i) {
		dlclose(op_arr[i].dlhandle);
	}

	return 0;
}

