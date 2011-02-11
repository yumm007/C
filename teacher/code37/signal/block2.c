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
	sigset_t nset, oset, saveset, origsave;
	void *save;

	save = signal(SIGINT, int_handler);

	sigemptyset(&nset);
	sigaddset(&nset, SIGINT);

	sigprocmask(SIG_UNBLOCK, &nset, &origsave);

	sigprocmask(SIG_BLOCK, &nset, &oset);

	for (i=0;;++i) {
		for (j=0;j<5;++j) {
			putchar('x');
			fflush(stdout);
			sleep(1);
		}
		putchar('\n');
		fflush(stdout);
/*
		sigprocmask(SIG_SETMASK, &oset, &saveset);
		pause();
		sigprocmask(SIG_SETMASK, &saveset, NULL);
*/
		sigsuspend(&oset);
	}

	sigprocmask(SIG_SETMASK, &origsave, NULL);

	signal(SIGINT, save);

	return 0;
}

