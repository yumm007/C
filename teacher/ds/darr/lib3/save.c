#include <stdio.h>

#include "darr.h"

#define NAMESIZE 32
struct score {
      int id;
      char name[NAMESIZE];
      int math;
};

void print_s(void *data)
{
      struct score *d = data;

      printf("%d, %s, %d\n", d->id, d->name, d->math);
}

int main(void)
{
      struct score tmp, *datap;
      DARR *arr;
      int i, ret, id;

      arr = darr_creat(sizeof(tmp));

      for (i = 0; i < 6; i++) {
	    tmp.id = i;
	    tmp.math = 100 - i;
	    snprintf(tmp.name, NAMESIZE, "stu%d", i);

	    ret = darr_append(arr, &tmp);
	    if (ret == -1) {
		  break;
	    }
      }

      darr_travel(arr, print_s);
      darr_save(arr, "tmp");

      darr_destroy(arr);
      return 0;
}
