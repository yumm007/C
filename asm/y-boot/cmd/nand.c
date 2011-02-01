#include <ushell.h>
#include <ustring.h>
#include <unand.h>

int nand(int argc, char **argv);
struct buidin_fun nand_fun  __attribute__ ((unused,section (".fun_section"))) = {"nand",nand};

int nand(int argc, char **argv) {
	int ret = -1;

	if (argc == 5 && str_cmp(argv[1], "read") == 0) {
		ret = nand_read(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
		puts("read complete.");
	} else if(argc == 5 && str_cmp(argv[1], "write") == 0) {
		ret = nand_write(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
		puts("write complete.");
	} else if(argc == 4 && str_cmp(argv[1], "erase") == 0) {
		ret = nand_erase(atoi(argv[2]), atoi(argv[3]));
		puts("erase complete.");
	} else 
		puts("avgv wrong.");
	return ret;
}

