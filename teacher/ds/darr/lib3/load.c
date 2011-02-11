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
      DARR *arr;

      arr = darr_load("tmp");

      printf("in load.c\n");
      darr_travel(arr, print_s);

      darr_destroy(arr);
      return 0;
}
