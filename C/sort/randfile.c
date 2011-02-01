#include <stdio.h>

#define MAXNUM 257

int main(void)
{
      FILE *fp;
      unsigned int i, tmp;

      fp = fopen("tmp", "w");
      /* if error */

      for (i = 0; i < MAXNUM; i++) {
            tmp = rand();
            fwrite(&tmp, sizeof(int), 1, fp);
            /* if error */
      }   

      fclose(fp);
      return 0;
}
