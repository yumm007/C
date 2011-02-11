#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>

static jmp_buf env;

static void
func_b(void)
{
	puts("func_b()");
	longjmp(env, 9);
	puts("func_b return.");
}

static void
func_a(void)
{
	int ret;

	puts("func_a()");
	if ((ret=setjmp(env))==0) {
		puts("jmp_buf OK.");
		func_b();
		puts("Returned from func_b()");
	} else {
		printf("Jumped back to func_a with code %d\n", ret);
	}
}

int
main()
{
	puts("main()");
	puts("Call func_a.");
	func_a();
	puts("Returned from func_a.");
	return 0;
}

