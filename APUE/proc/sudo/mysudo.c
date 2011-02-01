#define _XOPEN_SOURCE
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/types.h>
#include <shadow.h>
#include <pwd.h>

#define	TRY	3
#define	SUDOFILE	"sudo.conf"

sigjmp_buf env;

static char *get_sha(void) {
	struct passwd *user_pass;
	struct spwd *user_shad;

	if ((user_pass = getpwuid(getuid())) == NULL) 
		return NULL;
	if ((user_shad = getspnam(user_pass->pw_name)) == NULL) 
		return NULL;
	return user_shad->sp_pwdp;
}

static void alrm_hanlder(int s) {
	siglongjmp(env, 1);
}

static int check_pass(const char *shadow) {
	const char *pass;
	void (*old_hanlder)(int);

	alarm(3);
	old_hanlder = signal(SIGALRM, alrm_hanlder);
	if (sigsetjmp(env, 1) == 0) {
		if ((pass = getpass("[mysudo] password: ")) == NULL) {
			fprintf(stderr, "get passwd error.\n");
			return -1;
		}
	} else {
		fprintf(stderr, "timeout.\t");
		signal(SIGALRM, old_hanlder);
		alarm(0);
		return -1;
	}

	signal(SIGALRM, old_hanlder);
	alarm(0);
   return strcmp(crypt(pass, shadow), shadow);
}

static int very_user_cmd_line(const char *real_name, const char *short_cmd, char *line) {
	char *user_name, *able_cmd;

	if ((user_name = strtok(line, ":")) == NULL)
		return -1;
	if (strcmp(user_name, real_name) != 0)
		return -1;
	
	while ((able_cmd = strtok(NULL, ": \t\n")) != NULL)
		if (strcmp(able_cmd,"ALL") == 0 || strcmp(able_cmd, "all") == 0 \
			|| strcmp(able_cmd, short_cmd) == 0)
			return 0;

	return -1;
}

static int very_user_cmd(const char *filename, const char *cmd) {
	struct passwd *user_pass;
	char *line = NULL;
	const char *short_cmd;
	FILE *fp;
	size_t linesize = 0;
	int findit = -1;

	if ((user_pass = getpwuid(getuid())) == NULL) 
		return -1;

	if ((fp = fopen(filename, "r")) == NULL)
		return -1;

	if ((short_cmd = strrchr(cmd,'/')) == NULL)
		short_cmd = cmd;
	else
		short_cmd++;

	while (findit == -1 && getline(&line, &linesize,fp) > 1) 
		findit = very_user_cmd_line(user_pass->pw_name, short_cmd, line);

	fclose(fp);
	return findit;
}

int main(int argc, char *argv[])
{
	int i;
	char *sha;
	struct termios saveter;

	if (argc < 2) {
		fprintf(stderr, "usage: %s command\n",argv[0]);
		return -1;
	}
	
	tcgetattr(0, &saveter);
	
	if (very_user_cmd(SUDOFILE, argv[1]) == -1) {
		fprintf(stderr, "you are not sudo user or command denied\n");
		return -1;
	}

	if ((sha = get_sha()) == NULL) {
		fprintf(stderr, "get shadow error.\n");
		return -1;
	}
	
	for (i = 0; i < TRY; i++) {
		if (getuid() == 0 || check_pass(sha) == 0){
			execvp(argv[1], &argv[1]);
			perror("mysudo:");
			break;
		}
		printf("Sorry, try again.\n");
	}

	tcsetattr(0, TCSANOW, &saveter);
	return 0;
}
