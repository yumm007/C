#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

#define LEFT 30000000
#define RIGHT 30000200

static sigjmp_buf save0;

static void
alrm_handler(int s)
{
	puts("ALARM!");
	fflush(stdout);
	siglongjmp(save0, 1);
	return;
}

static int
do_main()
{
	int i, j, mark;

	for (i=LEFT;i<=RIGHT;++i) {
		mark = 1;
		for (j=2;j<i/2;++j) {
			if (i%j==0) {
				mark=0;
				break;
			}
		}
		if (mark==1) {
			printf("%d is a prime.\n", i);
		}
	}

	return 0;
}

int
main()
{
	signal(SIGALRM, alrm_handler);

	if (sigsetjmp(save0, 1)==0) {
		alarm(1);
		do_main();
		alarm(0);
	} else {
		puts("Timed out!");
	}

	return 0;
}

