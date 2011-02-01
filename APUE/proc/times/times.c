#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/times.h>

int main(int argc, char *argv[])
{
	clock_t start_time, end_time;
	struct tms tms_bef, tms_end;
	pid_t pid;
	long clock;

	clock = sysconf(_SC_CLK_TCK);
	
	start_time = times(&tms_bef);
	if ((pid = fork()) < 0) {
		perror("fork");
		return -1;
	} else if (pid == 0) {
		execvp(argv[1], &argv[1]);
	}
	wait(NULL);	
	end_time = times(&tms_end);

	printf("real time: %.4f\n", (end_time - start_time) / (double) clock);
	printf("user time: %.4f\n", ((tms_end.tms_utime - tms_bef.tms_utime) + \
			(tms_end.tms_cutime - tms_bef.tms_cutime)) / (double) clock);
	printf("sys time: %.4f\n", (tms_end.tms_stime - tms_bef.tms_stime) + \
			 (tms_end.tms_cstime - tms_bef.tms_cstime)/ (double) clock);

	return 0;
}
