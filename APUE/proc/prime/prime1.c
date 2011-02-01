#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define 	LEFT	50000000
#define 	RIGHT	50050202
#define 	N		6

static void is_prime(int left, int right) {
	int i, j, flag;
	
	//printf("%d: %d - %d , total: %d \n", getpid(), left, right, right - left + 1);
	for (i = left; i <= right; i++) {
		flag = 1;
		for (j = 2; flag && j < i / 2;j++)
			flag =  i % j;
		if (flag)	
			printf("%d is prime\n", i);
	}

	return;
}

int main(void) {
	int i, l, r, step, add, rem;
	pid_t pid;

	step = (RIGHT - LEFT + 1) / N;
	rem = (RIGHT - LEFT + 1) % N;
	add = 0;
	r = LEFT - 1;
		
	for (i = 0; i < N; i++) {
		l = r + 1;
		r = l + step - 1;
		if (add++ < rem)
			r++;
		if ((pid = fork()) < 0)
			return -1;
		else if (pid == 0){
			is_prime(l, r);
			return -1;
		}
	}

	for (i = 0; i < N; i++)
		wait(NULL);

	return 0;
}
