#include <stdio.h>
#include <stdlib.h>

#include "anytimer.h"

static void
func1(void *ptr)
{
	printf("func1(): %s\n", ptr);
}

static void
func2(void *ptr)
{
	printf("func2(): %s\n", ptr);
}

int
main()
{
	int job1, job2, job3, i;

	puts("Begin.");
	job1 = at_add_job(5, func1, "aaa");
	job2 = at_add_job(2, func2, "bbb");
	job3 = at_add_job(9, func1, "ccc");
	puts("Over.");
	for (i=0;i<15;++i) {
		putchar('x');
		fflush(stdout);
		sleep(1);
	}
	at_wait_job(job1);
	at_wait_job(job2);
	at_wait_job(job3);
	return 0;
}

