#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define	PARR	1
#define	CHLD	2

static int *i = NULL;
static int *next = NULL;
sigset_t unlock_usr1, lock_usr1, save_set;

static void pid_fun(void) {
	int p = 0;	

	while (*i != -1) {
		while (*next != CHLD)
			sigsuspend(&unlock_usr1);
		p += *i;	
		*next = PARR;
		kill(getppid(), SIGUSR1);
	}

	printf("p = %d\n", p);
}

void do_nothing(int unused) {};

int main(void) {
	pid_t pid;
	int j;
	struct sigaction sig_han, old_sig;


	sigemptyset(&unlock_usr1);	
	sigemptyset(&lock_usr1);	
	sigemptyset(&save_set);	
	sigaddset(&lock_usr1, SIGUSR1);

	sigprocmask(SIG_UNBLOCK, &lock_usr1, &save_set);
	sigprocmask(SIG_BLOCK, &lock_usr1, &unlock_usr1);	

	sig_han.sa_handler = do_nothing;
	sigemptyset(&sig_han.sa_mask);
	sig_han.sa_flags = 0;

	sigaction(SIGUSR1, &sig_han, &old_sig);

	if ((next = mmap(NULL, sizeof(*i) * 2, PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
		fprintf(stderr, "mmap()\n");
		exit(1);
	}

	i = next + 1;
	*i = 0;

	*next = PARR;
	sigprocmask(SIG_SETMASK, &unlock_usr1, NULL);

	if ((pid = fork()) < 0)
		exit(1);
	else if (pid == 0) {
		pid_fun();
		exit(1);
	}
	
	for (j = 0; j < 100; j++) {
		*i = j;
		*next = CHLD;
		kill(pid, SIGUSR1);
		while (*next != PARR)
			sigsuspend(&unlock_usr1);
	}
	*i = -1;

	wait(NULL);
	sigprocmask(SIG_SETMASK, &save_set, NULL);
	sigaction(SIGUSR1, &old_sig, NULL);

	return 0;
}

