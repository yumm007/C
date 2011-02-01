#include <common.h>
#include <command.h>

static int parse_line(char *line, char *argv[])
{
	int argc = 0;

	while(argc < CONFIG_SYS_MAXARGS)
	{
		while((*line == ' ') || (*line == '\t'))
			++line;

		if(*line == '\0')
		{
			argv[argc] = NULL;
			return argc;
		}

		argv[argc++] = line;

		while(*line && (*line != ' ') && (*line != '\t'))
			++line;

		if(*line == '\0')
		{
			argv[argc] = NULL;
			return argc;
		}

		*line++ = '\0';
	}

	printf ("** Too many args (max. %d) **\n", CONFIG_SYS_MAXARGS);

	return argc;
}

static cmd_tbl_t *find_cmd(const char *cmd)
{
	cmd_tbl_t *start;
	cmd_tbl_t *end;

	start = &__g_boot_cmd_start;
	end = &__g_boot_cmd_end;
	
	while(start < end){
		if(strcmp(start->name, cmd) == 0)
			return start;
		start++;
	}

	return NULL;
}


int exec_command(const char *cmd)
{
	cmd_tbl_t *cmdtp;
	char cmdbuf[CONFIG_SYS_CBSIZE + 1];
	char *argv[CONFIG_SYS_MAXARGS + 1];
	int argc;

	if (!cmd || !*cmd)
		return -1;

	strcpy(cmdbuf, cmd);

	if((argc = parse_line(cmdbuf, argv)) == 0)
		return -1;

	if((cmdtp = find_cmd(argv[0])) == NULL){
		printf("%s: command not found\n",argv[0]);
		return -1;
	}

	if(cmdtp->cmd(argc, argv) != 0)
		return -1;

	return 0;
}

void shell(void)
{
	static char cmd[CONFIG_SYS_CBSIZE + 1];
	int len;

	for(;;){
		printf(CONFIG_SYS_PROMPT" ");

		len = gets(cmd);

		if(len > 0)
			exec_command(cmd);
	}
}


