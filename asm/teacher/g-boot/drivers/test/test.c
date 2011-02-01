#include <common.h>
#include <command.h>

int do_test(int argc, char *argv[])
{
	puts("hello test....");

	return 0;
}


G_BOOT_CMD(test, do_test);
