#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static sig_atomic_t loop=1;

static void
alrm_handler(int s)
{
	loop=0;
}

int
main()
{
	struct sigaction sa;

	sa.sa_handler = alrm_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGALRM, &sa, NULL);

	alarm(5);

	while (loop) {
		putchar('x');
	}

	return 0;
}

