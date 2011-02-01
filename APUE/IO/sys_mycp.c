#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef BUFSIZE
#define BUFSIZE 32
#endif
#define TIMEOUT 120

int main(int argc, char **argv)
{
		int fdr, fdw;
      char buf[BUFSIZE];
      int retr, sec, flag = 1, wc;

      if (argc != 3) {
            fprintf(stderr, "argment...\n");
            return 1;
      }

      fdr = open(argv[1], O_RDONLY);
      if (fdr == -1) {
            perror(argv[1]);
            return 1;
      }

      fdw = open(argv[2], O_CREAT|O_RDWR|O_TRUNC, 0666);
      if (fdw == -1) {
            perror("open file for wrire faild");
            close(fdr);
            return 1;
      }

      while (flag) {
            retr = read(fdr, buf, BUFSIZE);
            if (retr == 0) {
                  break;
            }
				sec = 0;
				wc = 0;
				while (retr > 0) {
            	wc = write(fdw, &buf[wc], retr);
					retr -= wc;
					sleep(sec);
					sec += 1;
					if (sec >= TIMEOUT) {
						flag = 0;
						fprintf(stderr, "write to %s timeout.\n", argv[2]);
						break;
					}
				}
      }
	
		fsync(fdw);

      close(fdw);
      close(fdr);

      return 0;
}
