#include <ushell.h>
#include <ustring.h>

int test(int argc, char **argv);
struct buidin_fun test_fun  __attribute__ ((unused,section (".fun_section"))) = {"test",test};

int test(int argc, char **argv) {
	puts("this is test line.");
	return 0;
}

