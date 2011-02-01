#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "relayer.h"

#define TTY1 "/dev/tty9"
#define TTY2 "/dev/tty10"
#define TTY3 "/dev/zero"
#define TTY4 "/dev/zero"

int
main()
{
	int job1, job2;
	int fd1, fd2;
	struct relayer_stat_st statres;
	int over, ret, sec = 0;

	fd1 = open(TTY1, O_RDWR|O_NOCTTY);
	if (fd1<0) {
		perror("open(TTY1)");
		exit(1);
	}
	write(fd1, "TTY1\n", 5);
	fd2 = open(TTY2, O_RDWR|O_NOCTTY);
	if (fd2<0) {
		perror("open(TTY2)");
		exit(1);
	}
	write(fd2, "TTY2\n", 5);

	job1 = relayer_addjob(fd1, fd2, 0);
	if (job1<0) {
		fprintf(stderr, "add job 1 error. return value: %d\n", job1);
		exit(1);
	}

	sleep(5);

	fd1 = open(TTY3, O_RDWR|O_NOCTTY);
	if (fd1<0) {
		perror("open(TTY3)");
		exit(1);
	}
	write(fd1, "TTY3\n", 5);
	fd2 = open(TTY4, O_RDWR|O_NOCTTY);
	if (fd2<0) {
		perror("open(TTY4)");
		exit(1);
	}
	write(fd2, "TTY4\n", 5);

	job2 = relayer_addjob(fd1, fd2, 0);
	if (job2<0) {
		fprintf(stderr, "Error.\n");
		exit(1);
	}
	over=0;
	while (1) {
		sec++;
		if (sec == 20)
			relayer_canceljob(job2);
		if (sec == 30)
			relayer_waitjob(job2);

		ret = relayer_statjob(job1, &statres);
		if (ret!=0) {
			fprintf(stderr, "job1, Error.\n");
		} else if (statres.over == 1){
			break;
		} else
			fprintf(stderr, "job1: 1->2=%lld\t2->1=%lld\t", statres.count12, statres.count21);
		
		ret = relayer_statjob(job2, &statres);
		if (ret!=0) {
			fprintf(stderr, "job2 Error.\n");
		} else 
			fprintf(stderr, "job2: 1->2=%lld\t2->1=%lld\n", statres.count12, statres.count21);
		
		sleep(1);
	}

	ret = relayer_waitjob(job1);
	printf("wait job 1 ret = %d\n", ret);
	ret = relayer_waitjob(job2);
	printf("wait job 2 ret = %d\n", ret);

	relayer_finished();
	while (1) {
		putchar('x');
		fflush(NULL);
		sleep(1);
	}
	
	return 0;
}

