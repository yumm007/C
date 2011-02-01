#include "myshell.h"

#define DELIM	" \t"

struct buildin_fun *fun_arr = NULL;
struct proc_grp grp;

static int init_one_cmd(char *line, glob_t *glob_arg, struct cmd_line *cmd) {
	static int index, glob_ind;
	static char *ret[CMD_MAX_LEN], *arg;
	char *arg_tok;
	int i; 
	static int glob_mode;

	if (glob_arg->gl_pathc == 0) {
		index = 0;
		glob_ind = 0;
		glob_mode = GLOB_NOSORT | GLOB_NOCHECK;
	}

	while ((arg = strtok_r(line, DELIM, &arg_tok))!= NULL) {
		if (arg[0] == '$')
			if ((arg = getenv(&arg[1])) == NULL)
				break;
		if (glob(arg, glob_mode, NULL, glob_arg) != 0) {
			perror(arg);
			return -1;
		}
		glob_mode = GLOB_NOSORT | GLOB_NOCHECK | GLOB_APPEND;
		line = NULL;
	}
	for (i = index; glob_ind < glob_arg->gl_pathc && i < CMD_MAX_LEN - 1; i++) 
		ret[i] = glob_arg->gl_pathv[glob_ind++];

	ret[i] = NULL;
	cmd->argc = i - index;
	cmd->argv = &ret[index];
	index = i + 1;

	return 0;
}

static int formatting_cmd(char *line, glob_t *glob_arg, struct cmd_line *cmd_arr) {
	char *arg, *arg1, *save_arg;
	int i = 0;
	
	if ((arg = strtok_r(line, "|\n", &save_arg)) == NULL) 
		return -1;
	do {
		if ((arg1 = strdup(arg)) == NULL)
			return -1;
		if (init_one_cmd(arg1, glob_arg, &cmd_arr[i++]) == -1)
			break;
		free(arg1);
	} while (i < CMD_ARR_LEN && (arg = strtok_r(NULL, "|\n", &save_arg)) != NULL);
	
	cmd_arr[i].argc = 0;
	cmd_arr[i].argv = NULL;

	return 0;
}

static void shell_prompt(void) {
	char *pwd = "", *tmp;
	if ((pwd = get_current_dir_name()) != NULL) 
		if (strlen(pwd) != 1 && (tmp = strrchr(pwd, '/')) != NULL)
			pwd = tmp+1;
	printf("[myshell:%s]$ ",pwd);
	fflush(stdout);
}

void bye(void) {
	fflush(NULL);
	free_buildin_fun(fun_arr);
}

static void send_sig(int sig) {
	if (grp.act_pid != getpid())
		kill(grp.act_pid, sig);
	else {
		printf("\n");
		shell_prompt();
	}
}

int main(void) {
	size_t line_size = 0;
	char *line = NULL;
	struct cmd_line *cmd_arr = NULL;
	glob_t glob_arg;

	grp.act_pid = getpid();
	atexit(bye);
	load_buildin_fun(&fun_arr);

	signal(SIGINT, send_sig);

	if ((cmd_arr = malloc(sizeof(*cmd_arr) * (CMD_ARR_LEN + 1))) == NULL) {
		fprintf(stderr, "malloc failed.\n");
		return -1;
	}

	while (1) {
		shell_prompt();
		getline(&line, &line_size, stdin);
		glob_arg.gl_pathc = 0;
		if ((formatting_cmd(line, &glob_arg, cmd_arr)) == 0) {
			exec_cmd(cmd_arr);
			if (glob_arg.gl_pathc > 0) globfree(&glob_arg);
		} 
	}
	free(line);
	free(cmd_arr);

	return 0;
}

