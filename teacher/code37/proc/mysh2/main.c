#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>
#include <sys/types.h>

#include "main.h"
#include "buildin.h"

struct mysh_cmd_st cmd;

static void
prompt(void)
{
	printf("mysh-0.1$ ");
}

static int
parse_line(struct mysh_cmd_st *res, char *linestr)
{
	int i, ret;
	char *tok;

	for (i=0;;++i) {
		tok = strsep(&linestr, DELIMS);
		if (tok==NULL) {
			break;
		}
		if (tok[0]=='\0') {
			--i;
			continue;
		}
		if (i==0) {
			ret = glob(tok, GLOB_NOCHECK, NULL, &res->glob_res);
		} else {
			ret = glob(tok, GLOB_NOCHECK|GLOB_APPEND, NULL, &res->glob_res);
		}
		if (ret) {
			return ret;
		}
	}
	return 0;
}

int
main()
{
	int i, ret;
	char *lineptr;
	size_t linesize;
	pid_t pid;

	lineptr=NULL;
	linesize=0;
	while (1) {
		prompt();
		getline(&lineptr, &linesize, stdin);
		if (0) {
			break;
		}
		if (lineptr[0]=='\n') {
			continue;
		}
		if (parse_line(&cmd, lineptr)==0) {
#if 0
			for (i=0;i<cmd.glob_res.gl_pathc;++i) {
				printf("argv[%d]=\"%s\"\n", i, cmd.glob_res.gl_pathv[i]);
			}
#endif
			if ((ret=is_buildin(&cmd))>=0) {
				do_buildin(ret, &cmd);
			} else {
				pid = fork();
				if (pid<0) {
				}
				if (pid==0) {
					if (in>=0) {
						dup2(in, 0);
						if (in!=0)  close(in);
					}
					if (out>=0) {
						dup2(out, 1);
						if (out!=1)  close(out);
					}
					if (err>=0) {
						dup2(err, 2);
						if (err!=2)  close(err);
					}
					execvp(cmd.glob_res.gl_pathv[0], cmd.glob_res.gl_pathv);
					perror("execvp()");
					return 1;
				}
				wait(NULL);
			}
		} else {
			fprintf(stderr, "glob failed.\n");
		}
		globfree(&cmd.glob_res);
	}
}

