#include <ushell.h>
#include <ustring.h>

int echo(int argc, char **argv);
struct buidin_fun echo_fun  __attribute__ ((unused,section (".fun_section"))) = {"echo",echo};

int echo(int argc, char **argv) {
	int i;
	char *c;
	
	for (i = 1; i < argc; i++) {
		c = argv[i];
		while (*c) {
			putchar(*c);
			c++;
		}
		putchar(' ');
	}
	putchar('\n');
	return 0;
}

