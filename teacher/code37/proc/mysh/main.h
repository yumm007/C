#ifndef MAIN_H
#define MAIN_H

#define DELIMS " \t\n"
/* static const char *DELIM=" \t\n"; */

struct mysh_cmd_st {
	glob_t glob_res;
	int out, in, err;
};

/*
struct mysh_job_st {
	int jobid;
	pid_t pgid;
	struct mysh_cmd_st *cmd;
};
*/

#endif

