#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>

int
main(int argc, char **argv)
{
	struct spwd *shadow_rec;
	char *pass;
	char *sp;

	if (argc<2) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	shadow_rec = getspnam(argv[1]);
	if (shadow_rec==NULL) {
		fprintf(stderr, "Unknown user.\n");
		exit(1);
	}

	pass = getpass("Password: ");
	if (pass==NULL) {
		perror("getpass()");
		exit(1);
	}

	sp = crypt(pass, shadow_rec->sp_pwdp);
	if (strcmp(sp, shadow_rec->sp_pwdp)==0) {
		printf("PASS.\n");
	} else {
		printf("FAIL.\n");
	}
	exit(0);
}

