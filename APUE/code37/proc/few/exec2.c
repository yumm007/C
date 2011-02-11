#include <stdio.h>
#include <unistd.h>

/* sleep 1000 */

#define TARGETNAME "httpd"

int
main(int argc, char **argv)
{
	if (strcmp(argv[0], TARGETNAME)!=0) {
		setenv("REALARGV", argv[1], 1);
		readlink("/proc/$PID/exe", buf);
		execl(buf, TARGETNAME, NULL);
		puts("After exec.");
		perror("execl()");
		return 1;
	}
	getenv("REALARGV");
	puts("OK!");
	sleep(1000);
	return 0;
}

