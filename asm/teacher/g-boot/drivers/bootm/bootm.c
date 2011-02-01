#include <common.h>
#include <command.h>
#include <bootm.h>

static struct tag *params;

static void setup_start_tag(void)
{
	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}

static void setup_commandline_tag(char *commandline)
{
	char *p;

	if (!commandline)
		return;

	for (p = commandline; *p == ' '; p++);

	if (*p == '\0')
		return;

	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;

	strcpy(params->u.cmdline.cmdline, p);

	params = tag_next(params);
}

static void setup_end_tag(void)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}


static int do_bootm(int argc, char *argv[])
{
	void (*kernel)(int, int, int);
	
	if(argc != 2)
		return -1;

	kernel = (void *)(atoh(argv[1]) + 0x40);
	
	params = (void *)0x30000100;

	setup_start_tag();
	
	setup_commandline_tag(CONFIG_BOOTARGS);
	
	setup_end_tag();

	printf ("\nStarting kernel ...\n\n");

	sleep(5);

	kernel(0, 362, 0x30000100);

	return 0;
}

G_BOOT_CMD(bootm, do_bootm);



