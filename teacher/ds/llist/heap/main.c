#include <stdio.h>

#include "mymem.h"

#define MYMEM_SIZE 1024
char mymem[MYMEM_SIZE];

int main(void)
{
      MEM *mem;
      void *arr[8];
      int i;

      mem = mymem_init(mymem, MYMEM_SIZE);

      printf("mymem = %p\n", mymem);
      for (i = 0; i < 8; i++) {
	    arr[i] = mymem_alloc(mem, 128);
	    printf("arr[%d] = %p\n", i, arr[i]);
      }

      for (i = 3; i < 6; i++) {
	    mymem_free(mem, arr[i]);
      }
      for (i = 3; i < 6; i++) {
	    arr[i] = mymem_alloc(mem, 64);
            printf("arr[%d] = %p\n", i, arr[i]);
      }
      printf("%p\n", mymem_alloc(mem, 32));

      mymem_clean(mem);
      return 0;
}
