#include "myshell.h"

extern struct buildin_fun *fun_arr;
extern struct proc_grp grp;

struct std_err {
	char *std_out_file;
	char *std_out_mode;
	char *std_err_file;
	char *std_err_mode;
};

static int init_std_err_file(const struct cmd_line *cmd, struct std_err *ret) {
	int i, cut = 0;
	char *c, *p;

	for (i = 1; (p = cmd->argv[i]) != NULL; i++) {
		while (*(c = p++) != '\0') 
			if (*c == '>') {
				cut = i;
				if (c != cmd->argv[i] && *(c - 1) == '2'){
					if (*p == '>') {
						ret->std_err_mode = "a";
						p++;
					} else
						ret->std_err_mode = "w";
					ret->std_err_file = (*p == '\0') ? cmd->argv[i+1] : p;
				} else {
					if (*p == '>') {
						ret->std_out_mode = "a";
						p++;
					} else
						ret->std_out_mode = "w";
					ret->std_out_file = (*p == '\0') ? cmd->argv[i+1] : p;
				}
				break;
			}
	}

	return cut;
}

static void exec_one_cmd(struct cmd_line *cmd) {
	/*只处理单条命令的重定向*/
	FILE *std_out, *std_err;
	struct std_err ret = {NULL,NULL,NULL,NULL};
	int cut;

	cut = init_std_err_file(cmd, &ret);

	if (ret.std_out_file != NULL && ret.std_out_file[0] != '&') {
		if ((std_out = fopen(ret.std_out_file, ret.std_out_mode)) == NULL) 
			perror(ret.std_out_file);
		else {
			dup2(fileno(std_out), cmd->fd[1]);
			dup2(cmd->fd[1], 1);
			fclose(std_out);
		}
	}
	if (ret.std_err_file != NULL && ret.std_err_file[0] != '&') {
		if ((std_err = fopen(ret.std_err_file, ret.std_err_mode)) == NULL) 
			perror(ret.std_err_file);
		else {
			dup2(fileno(std_err), cmd->fd[2]);
			dup2(cmd->fd[2], 2);
			fclose(std_err);
		}
	}

	if (cut != 0)
		cmd->argv[cut] = NULL;
	execvp((cmd->argv)[0], cmd->argv);
	perror((cmd->argv)[0]);
	exit(-1);
}

void travel_cmd_arr(const struct cmd_line *cmd_arr) {
	int i, j;
	char **p;
	
	for (i = 0; (p = cmd_arr[i].argv) != NULL; i++){
		for (j = 0; p[j] != NULL; j++)
			printf("%s ", p[j]);
		printf("\n");
	}
	fflush(NULL);
}

static void init_arr_fd(struct cmd_line *cmd_arr) {
	int i, fd[2];
	fun_fmt fun;
	
	/*只处理管道*/
	cmd_arr[0].fd[0] = dup(0);
	for (i = 1; cmd_arr[i].argv != NULL; i++) {
		if ((fun = is_buildin_fun(fun_arr, cmd_arr[i].argv[0])) != NULL) {
			fun(cmd_arr[i].argc, cmd_arr[i].argv);
			continue;
		}
		if (pipe(fd) == -1) {
			perror("pipe()");
			exit(-1);
		}		
		cmd_arr[i-1].fd[1] = dup(fd[1]);
		cmd_arr[i-1].fd[2] = dup(2);
		cmd_arr[i].fd[0] = dup(fd[0]);
		close(fd[0]);
		close(fd[1]);
	}
	cmd_arr[i-1].fd[1] = dup(1);
	cmd_arr[i-1].fd[2] = dup(2);

	return;
}

static void free_fd_arr(struct cmd_line *cmd_arr) {
	int fd0, fd1, i;

	for (i = 0; cmd_arr[i].argv != NULL; i++) {
		if ((fd0 = cmd_arr[i].fd[0]) > 2) close(fd0);
		if ((fd1 = cmd_arr[i].fd[1]) > 2) close(fd1);
	}
}

static void shell_exit(int argc, char *argv[]) {
	if (argc > 1)
		exit(atoi(*++argv));
   exit(0);
}

void exec_cmd(struct cmd_line *cmd_arr) {
	fun_fmt fun;
	int i, status;
	pid_t pid;

	init_arr_fd(cmd_arr);

	for (i = 0; cmd_arr[i].argv != NULL; i++) {
		fflush(NULL);
		if (strcmp(cmd_arr[i].argv[0],"exit") == 0) 
			shell_exit(cmd_arr[i].argc, cmd_arr[i].argv);
		if ((fun = is_buildin_fun(fun_arr, cmd_arr[i].argv[0])) != NULL) {
			fun(cmd_arr[i].argc, cmd_arr[i].argv);
			continue;
		}
		if ((pid = fork()) < 0)	{
			perror("fork()");
			exit(-1);
		} else if (pid == 0) {
			signal(SIGINT, SIG_DFL);
			if (dup2(cmd_arr[i].fd[0], 0) == -1) perror("dup2:");
			if (dup2(cmd_arr[i].fd[1], 1) == -1) perror("dup2:");
			free_fd_arr(cmd_arr);
			exec_one_cmd(cmd_arr + i);
		} else {
			grp.act_pid = pid;
			do {
				wait(&status);
			} while (WIFSTOPPED(status) || WIFCONTINUED(status));
			close(cmd_arr[i].fd[0]);
			close(cmd_arr[i].fd[1]);
			close(cmd_arr[i].fd[2]);
			grp.act_pid = getpid();
		}
	}

	return;
}

