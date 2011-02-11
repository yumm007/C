#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <string.h>

#define LINESIZE 1024

int
main(int argc, char **argv)
{
	FILE *fp;
	time_t stamp;
	struct tm *broken_tm;
	int c;
	char fmt[LINESIZE], linebuf[LINESIZE];

	stamp = time(NULL);

	fp = stdout;
	fmt[0]='\0';
	while (1) {
		c = getopt(argc, argv, "-Y:MDh:ms");
		if (c==-1) {
			break;
		}
		switch (c) {
			case 1:
				if (fp!=stdout) fclose(fp);
				fp = fopen(argv[optind-1], "w");
				if (fp==NULL) {
					perror("fprintf()");
					fp = stdout;
				}
				break;
			case 'Y':
				if (strcmp(optarg, "4")==0) {
					strcat(fmt, "%Y ");
/* FIXME: Overflow BUG! */
				} else if (strcmp(optarg, "2")==0) {
					strcat(fmt, "%y ");
				} else {
					fprintf(stderr, "Bad year format.\n");
				}
				break;
			case 'M':
				strcat(fmt, "%m ");
				break;
			case 'D':
				strcat(fmt, "%d ");
				break;
			case 'h':
				if (strcmp(optarg, "24")==0) {
					strcat(fmt, "%H ");
				} else if (strcmp(optarg, "12")==0) {
					strcat(fmt, "%I(%P) ");
				} else {
					fprintf(stderr, "Bad hour format.\n");
				}
				break;
			case 'm':
				strcat(fmt, "%M ");
				break;
			case 's':
				strcat(fmt, "%S ");
				break;
			default:
				break;
		}
	}

	broken_tm = localtime(&stamp);
	strftime(linebuf, LINESIZE, fmt, broken_tm);

	fprintf(fp, "%s\n", linebuf);

	if (fp!=stdout) fclose(fp);

	return 0;
}

