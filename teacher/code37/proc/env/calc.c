#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

#define NROPS 5

typedef int op_func_t(int, int);
typedef struct {
	op_func_t *funcp;
	int sym;
	char *desc;
} op_t;

static op_func_t add, sub, mul, idiv, mod;

static op_t op_arr[NROPS] = {
	{add, '+', "Add"},
	{sub, '-', "Sub"},
	{mul, 'x', "Multiple"},
	{idiv, '/', "Integer divide"},
	{mod, '%', "Modular"},
};

#define OP(i) (op_arr[i].funcp)
#define SYM(i) (op_arr[i].sym)

int
main()
{
	int i1, i2;

	for (i1=0;i1<NROPS;++i1) {
	for (i2=0;i2<NROPS;++i2) {
		if (OP(i2)(OP(i1)(2, 4), 6)==2) {
			printf("(2%c4)%c6=2\n", SYM(i1), SYM(i2));
		}
	}
	}
	return 0;
}

static int
add(int a, int b)
{
	return a+b;
}
static int
sub(int a, int b)
{
	return a-b;
}
static int
mul(int a, int b)
{
	return a*b;
}
static int
idiv(int a, int b)
{
	return a/b;
}
static int
mod(int a, int b)
{
	return a%b;
}

