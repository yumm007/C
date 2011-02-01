#include <stdio.h>
#include <math.h>
#define MAXNUM 2000000
int main(void)
{
      int prime[MAXNUM] = {1,1,};
      int i, j;
	
	
      for (i = 2; i < sqrt(MAXNUM) + 1; i++) {
            if (prime[i] == 0) {
                  for (j = i + i; j < MAXNUM; j += i) {
                        prime[j] = 1;
                  }
            }
      }

      for (i = 0, j = 0; i < MAXNUM; i++) {
            if (prime[i] == 0) {
                  printf("%d ", i);
                  j++;
            }
      }
      printf("\n%d\n", j);

      return 0;
}

