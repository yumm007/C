#ifndef	_MYSHELL_H
#define	_MYSHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <glob.h>
#include <signal.h>

#define 	CMD_MAX_LEN			4096
#define	CMD_ARR_LEN			10
#define	PLUGIN_PATH			"plugin/*.plugin"

typedef int (*fun_fmt)(int argc, char *argv[]);
struct buildin_fun {
	char *fun_name;
	fun_fmt	fun_body;
	void *handler;
};

struct cmd_line {
	int argc;
	char **argv;
	int fd[3];
};

struct proc_grp {
	pid_t act_pid;
};

int reg_fun_arr(const char *filepath, struct buildin_fun *cur);
int load_buildin_fun(struct buildin_fun **fun_arr);
void free_buildin_fun(struct buildin_fun *cur);
fun_fmt is_buildin_fun(const struct buildin_fun *fun_arr, const char *cmd);
void exec_cmd(struct cmd_line *cmd_arr);
void travel_cmd_arr(const struct cmd_line *cmd_arr);


#endif
