#include <ushell.h>
#include <ustring.h>

#define PROMPT		"ushell: "
#define MAX_ARG	10
#define BUFSIZE	1014

#undef	USHELL_DEBUG

static void print_prompt(void);
static int transition_arg(char **arg, char *buf); //从buf中截取命令以及参数放到指针数组arg中
static void run_cmd(int argc, char **argv);

extern struct buidin_fun fun_start;
extern struct buidin_fun fun_end;

//extern int bootm(int argc, char **argv);
//struct buidin_fun bootm_fun  __attribute__ ((unused,section (".fun_section"))) = {"bootm",bootm};

int ushell(void)
{
	char buf[BUFSIZE]; //输入的命令以及参数
	char *arg[MAX_ARG + 1];		//最大参数个数
	int n;

	while (1) {
		print_prompt();
		mem_set(buf, 0, BUFSIZE);
		gets(buf); //注意缓冲区问题
		n = transition_arg(arg, buf);
		if (str_cmp("exit", arg[0]) == 0) {
			puts("byebye.");
			break;
		}
		run_cmd(n, arg);
	}

	return 0;
}

static void run_cmd(int argc, char **argv) {
	struct buidin_fun *cmd_arr = &fun_start;
	int i = 0, findit = 0;
	
	while (cmd_arr + i != &fun_end) {
		if (str_cmp(argv[0], cmd_arr[i].name) == 0) {
			findit = 1;
			break;
		}
		i++;
	}

	if (findit) {
		(cmd_arr[i].fun)(argc, argv);
	}else
		puts("unknown command.");
}

static void print_prompt(void) {
	char *c = PROMPT;
	while (*c) {
		putchar(*c);
		c++;
	}
}

static int transition_arg(char **arg, char *buf) {
	int n;
	char *c = buf;
	
	n = 0;
	while (*c != '\0' && n < MAX_ARG) {
		while (*c == ' ' || *c == '\t') //跳过开始的空格或者tab键盘
			c++;
		arg[n] = c;		
		while (*c != ' ' && *c != '\t' && *c != '\0') //直接到后面的第一个空格或者tab键
			c++;
		if (*c != '\0') {
			*c = '\0'; //找到一个参数，参数尾置0
			c++;
		}
		n++;	
	}
	
	arg[n] = 0; //最后一个参数置为NULL
	return n;
}
