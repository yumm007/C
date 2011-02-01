#include <stdio.h>
#include <unistd.h>

#define BUFSIZE 32
#define TIMEOUT 120

int main(int argc, char **argv)
{
      FILE *fpr, *fpw;
      char buf[BUFSIZE];
      int retr, sec, flag = 1, wc;

      if (argc != 3) {
            fprintf(stderr, "argment...\n");
            return 1;
      }

      fpr = fopen(argv[1], "rb");
      if (fpr == NULL) {
            perror(argv[1]);
            return 1;
      }

      fpw = fopen(argv[2], "wb");
      if (fpw == NULL) {
            perror(argv[2]);
            fclose(fpr);
            return 1;
      }

      while (flag) {
            retr = fread(buf, 1, BUFSIZE, fpr);
            if (retr == 0) {
                  break;
            }
				sec = 0;
				wc = 0;
				while (retr > 0) {
            	wc = fwrite(&buf[wc], 1, retr, fpw);
					//fprintf(stderr, "write %d bytes.\n", wc);
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

      fclose(fpw);
      fclose(fpr);

      return 0;
}
