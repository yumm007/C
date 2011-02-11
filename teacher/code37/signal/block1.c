#include <stdio.h>
#include <signal.h>

static void
int_handler(int s)
{
	putchar('O');
	fflush(stdout);
}

int
main()
{
	int i, j;
	sigset_t nset, saveset;
	void *save;

	save = signal(SIGINT, int_handler);

	sigemptyset(&nset);
	sigaddset(&nset, SIGINT);

	sigprocmask(SIG_UNBLOCK, &nset, &saveset);
	for (i=0;i<5;++i) {
		sigprocmask(SIG_BLOCK, &nset, NULL);
		for (j=0;j<5;++j) {
			putchar('x');
			fflush(stdout);
			sleep(1);
		}
		putchar('\n');
		fflush(stdout);
		sigprocmask(SIG_UNBLOCK, &nset, NULL);
	}
	sigprocmask(SIG_SETMASK, &saveset, NULL);
	signal(SIGINT, save);

	return 0;
}

