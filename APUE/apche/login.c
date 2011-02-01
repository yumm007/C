//#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define 	MAXLINE 			4096
#define	LOGDIR			"/var/www/log/"
#define	PWFILE			"/var/www/passwd"
#define	NAME_PW_MAX		40
#define	NAME_MAX			20
#define	PW_MAX			NAME_PW_MAX - NAME_MAX - 1
#define	FILEMAX			256

static void output(const char *line, const char *username, FILE *fp) {
	int i = 0;
	char *linebuf = NULL; 
	size_t n, linesize = 0;
	int readc = 0;
	
	fprintf(fp, "%s>: ", username); /*将提示符和命令存入文件中*/
	while (line[i]!= '>')
		fputc(line[i++], fp);
	fputc('\n', fp);
	fflush(fp);

	system(line);	/*运行命令并将结果存入到文件中*/
	rewind(fp);

	while ((readc = getline(&linebuf, &linesize, fp)) > 0) {
		n = 0;
		while (n < readc) 
			n += fwrite(linebuf + n, 1, readc - n, stdout);
	}

}

static int get_shell(char *shell, size_t maxlen, const char *src) {
	char *cmd, f[3];
	unsigned int c;
	size_t len = 0;

	if ((cmd = strstr(src, "shell=")) == NULL) 
		return -1;

	cmd += strlen("shell=");
	while ((c = *cmd++) != '\0' && len < maxlen - 1) {
		if (c == '%') {
			f[0] = *cmd++;
			f[1] = *cmd++;
			f[2] = '\0';
			sscanf(f, "%x", &c);
		}
		shell[len++] = (c == '+' ? ' ': c);
	}
	shell[len] = '\0';

	return 0;
}

static int check_shell(const char *shell, const char *logname) {
	char cmd[MAXLINE];

	if (strcmp(shell, "clear") == 0) {
		strcpy(cmd, "rm -rf ");
		strcat(cmd, logname);
		system(cmd);
		return 0;
	}

	return -1;
}

static int convert_shell(char *line, const char *src, const char *logname) {

	if (get_shell(line, MAXLINE - strlen(logname) - strlen(" >> 2>&1") ,src) == -1)
		return -1; 
	if (check_shell(line, logname) == 0) /* 说明已经执行来内部命令*/
		return -1;

	strcat(line, " >>");
	strcat(line, logname);
	strcat(line, " 2>&1");
	
	return 0;
}

/* 在传入的字符串中获取用户名和密码 */
static int get_name_pw(char *username, char *password, const char *line) {
	char *name, *split, *passwd, *next_split;
	size_t name_len, passwd_len;

	if ((name = strstr(line, "username=")) == NULL) 
		return -1;
	name = name + strlen("username=");
	if ((split = strstr(line, "&")) == NULL) 
		return -1;
	name_len = (split - name > NAME_MAX -1 ? NAME_MAX -1: split - name);
	strncpy(username, name, name_len);
	username[name_len] = '\0';

	if ((passwd = strstr(line, "passwd=")) == NULL) 
		return -1;
	passwd = passwd + strlen("passwd=");
	if ((next_split = strstr(passwd, "&")) == NULL)
		passwd_len = (strlen(passwd) > PW_MAX -1) ? PW_MAX -1: strlen(passwd);
	else 
		passwd_len = (next_split - passwd > PW_MAX -1) ? PW_MAX -1: next_split - passwd;
	strncpy(password, passwd, passwd_len);
	password[passwd_len] = '\0';

	return 0;
}

/* 在传入的字符串中检查用户名和密码 */
static int check_user(const char  *username, const char *password, const char *pwfile) {
	char *linebufp = NULL, name_pw[NAME_PW_MAX];
	size_t linebufsize = 0;
	FILE *fp;

	if ((fp = fopen(pwfile, "r")) == NULL) 
		return -1;

	strcpy(name_pw, username);
	strcat(name_pw, ":");
	strcat(name_pw, password);
	strcat(name_pw, "\n");

	while (getline(&linebufp, &linebufsize, fp) > 0) 
		if (strcmp(name_pw, linebufp) == 0) /* 在密码文件中找到来完全匹配的记录 */
			return 0;
	
	return -1;
}

static void print_shell(const char *username, const char *password) {

	printf("<br><form action=\"/cgi-bin/login.cgi#tail\" method=\"POST\">");
	printf("<input type=\"hidden\" name=\"username\" value=\"%s\">", username);
	printf("<input type=\"hidden\" name=\"passwd\" value=\"%s\">", password);
	printf("%s@webshell:<input type=\"text\" name=\"shell\" size=80><br>", username);
	printf("<input type=\"submit\">");
	printf("</form>");
  	printf("<a name=\"tail\"></a>");

}

static void print_header(void) {
	printf("Content-type: text/html\r\n\r\n");
	printf("<html><head>");
	printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"/>");
	printf("<style>* { font-size:14px; } </style>");
	printf("<title>Web Shell</title>");
	printf("</head>");
	printf("<body>");
	printf("<pre>");
}

static void print_tail(void) {
	printf("</pre></body></html>");
}

int main()
{
	char *linebufp = NULL;
	size_t linebufsize = 0;
	char line[MAXLINE], username[NAME_MAX], password[PW_MAX], filename[FILEMAX];
	FILE *fp;
	
	print_header();
	getline(&linebufp, &linebufsize, stdin);

	if (get_name_pw(username, password, linebufp) == -1 || \
		check_user(username, password, PWFILE) != 0) {
			printf("login faild. <A href=\"/index.html\">back to login page</A><br>");	
			return -1;
	}

	strcpy(filename, LOGDIR); /* 将log文件目录加到文件名中*/
	strcat(filename, username); /* 每个用户都使用自己的log文件*/

	if (convert_shell(line, linebufp, filename) == 0) {
		if ((fp = fopen(filename, "a+b")) == NULL) {
			printf("open log file error.\n");
			return -1;
		}
		output(line, username, fp);
	}

	print_shell(username, password);
	print_tail();
	free(linebufp);

	return 0;
}

