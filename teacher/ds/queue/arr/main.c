#include <stdio.h>

#include "queue.h"

int main(void)
{
      int i, j, ret, data;
      int arr[] = {4,5,3,6,2,1,8};

      for (j = 0; j < 3; j++) {
	    for (i = 0; i < 4; i++) {
		  ret = queue_en(arr[i]);
		  if (ret == -1) {
			printf("queue full. data = %d\n", arr[i]);
			break;
		  }
	    }
	    for (i = 0; i < 5; i++) {
		  ret = queue_de(&data);
		  if (ret == -1) {
			printf("  queue empty.");
			break;
		  }
		  printf("%d ", data);
	    }
	    printf("\n");
      }

      return 0;
}
