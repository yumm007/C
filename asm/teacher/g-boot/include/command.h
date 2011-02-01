#ifndef _COMMAND_H_
#define _COMMAND_H_

typedef struct cmd_tbl{
	char *name;
	int (*cmd)(int argc, char *argv[]);
}cmd_tbl_t;

#define cmd_section  __attribute__ ((unused,section (".g_boot_cmd")))

#define G_BOOT_CMD(cmd_name, cmd_cmd)			\
							\
cmd_tbl_t __g_boot_cmd_##cmd_name cmd_section = 	\
{							\
	.name 	= #cmd_name, 				\
	.cmd	 = cmd_cmd,				\
}

/* g-boot.lds */
extern cmd_tbl_t __g_boot_cmd_start;
extern cmd_tbl_t __g_boot_cmd_end;

#endif
