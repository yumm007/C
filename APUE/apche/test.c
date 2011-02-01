#include <stdio.h>
#include <string.h>

#define	NAME_PW_MAX		40

/*拆离出用户名和密码并组合成与密码文件一致的格式  */
static int get_name_pw(char *buf, const char *line) {
	char *name, *split, *passwd;
	int len;

	if ((name = strstr(line, "username=")) == NULL)
		return -1;
	name = name + strlen("username=");
	if ((split = strstr(line, "&")) == NULL)
		return -1;
	if ((passwd = strstr(line, "passwd=")) == NULL)
		return -1;
	passwd = passwd + strlen("passwd=");

	strncpy(buf, name, split - name);
	strcpy(buf + (split - name), ":");
	strcat(buf, passwd);

	len = strlen(buf);
	buf[len] = '\n';
	buf[len + 1] = '\0';

	return 0;
}

static int check_user(const char *line, const char *pwfile) {
	char *linebufp = NULL, name_pw[NAME_PW_MAX];
	size_t linebufsize = 0;
	FILE *fp;

	if ((fp = fopen(pwfile, "r")) == NULL) {
		printf("open passwd file error.\n");
		return -1;
	}

	if (get_name_pw(name_pw, line) == -1)
		return -1;

	while (getline(&linebufp, &linebufsize, fp) > 0) 
		if (strcmp(name_pw, linebufp) == 0) /* 在密码文件中找到来完全匹配的记录 */
			return 0;
	
	return -1;
}


static int get_name_pw_v3(char *username, char*password, const char *line) {
	char *name, *split, *passwd, *next_split;

	if ((name = strstr(line, "username=")) == NULL) 
		return -1;
	name = name + strlen("username=");
	if ((split = strstr(line, "&")) == NULL) 
		return -1;
	if ((passwd = strstr(line, "passwd=")) == NULL) 
		return -1;
	passwd = passwd + strlen("passwd=");
	if ((next_split = strstr(passwd, "&")) == NULL) 
		return -1;

	strncpy(username, name, split - name);
	username[split - name] = '\0';
	strncpy(password, passwd, next_split - passwd);
	password[next_split - passwd] = '\0';

	return 0;
}

static int get_name_pw_v2(char *username, char *password, const char *line) {
	char *name, *split, *passwd, *next_split;
	char u[]="username=", p[]="passwd=";

	if ((name = strstr(line, u)) == NULL) {
		printf("get user name faild.\n");
		return -1;
	}
	name = name + strlen(u);
	if ((split = strstr(line, "&")) == NULL) 
		return -1;
	if ((passwd = strstr(line, "passwd=")) == NULL) 
		return -1;
	passwd = passwd + strlen("passwd=");
	if ((next_split = strstr(passwd, "&")) == NULL) 
		return -1;

	strncpy(username, name, split - name);
	username[split - name] = '\0';
	strncpy(password, passwd, next_split - passwd);
	password[next_split - passwd] = '\0';

	return 0;
}

int main(void)
{
	char line[] = "login.asp?username=user1&passwd=34343434&shell=ls";
	char buf[40], username[20], password[20];

	get_name_pw_v2(username, password, line);
	printf("username = %s, password = %s\n", username, password);

	return 0;
}
/*拆离出用户名和密码并组合成与密码文件一致的格式  */
