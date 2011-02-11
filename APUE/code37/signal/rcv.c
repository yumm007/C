#include <stdio.h>
#include <signal.h>

static void
usr1_handler(int s, siginfo_t *infop, void *ptr)
{
	if (infop->si_code == SI_USER) {
		printf("SIGUSR1 from %d caught.\n", infop->si_pid);
	} else if (infop->si_code == SI_QUEUE) {
		printf("SIGUSR1 with value %d from %d caught.\n", infop->si_value.sival_int, infop->si_pid);
	} else {
		printf("Unknown source SIGUSR1 caught.\n");
	}
}

int
main()
{
	struct sigaction sa;

	sa.sa_sigaction = usr1_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);

	while (1) pause();
}

