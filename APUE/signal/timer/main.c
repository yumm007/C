#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "anytimer.h"

#define TIM_MAX	50

extern void *tim_link;
static void print_s(const void *data) {
	const int *job = data;
	printf("job->num = %d, val = %d, rep = %d\n", job[1], job[0], job[3]);
}

static void
fun1(void *ptr)
{
	printf("func1(): %s\n",(char *)ptr);
}

void *pth_fun(void *ind)
{
	int i, j, ret;
	job_des job;

	char arg[1024];
	int job_arr[TIM_MAX];

	for (i = 1, j = 0; i < TIM_MAX; i++) {
		job.sec = i;
		job.rep_time = i;
		job.fun = fun1;
		sprintf(arg, "repert job in thread %d: job number %d wait %d's", (int)ind, i, i);
		job.arg = arg;
		job.arg_size = strlen((char *)job.arg) + 1;
		if ((ret = at_add_job(&job)) >= 0) {
			job_arr[i] = ret;
			j++;
		}
	}

	while (j) {
		sleep(1);
		for (i = 1; i< TIM_MAX; i++)
			if (at_get_sta(job_arr[i]) == 8) {
				at_wait_job(job_arr[i]);
				j--;
			}
	}

	pthread_exit(NULL);
}

int main(void)
{
	pthread_t pthd[TIM_MAX];
	int i;

	for (i = 0; i < TIM_MAX; i++) 
		pthread_create(pthd+i, NULL, pth_fun, (void *)i);
	for (i = 0; i < TIM_MAX; i++) 
		pthread_join(pthd[i], NULL);
	printf("travel again.\n");
	link_travel(tim_link, print_s);
	
	return 0;
}

