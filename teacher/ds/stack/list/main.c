#include <stdio.h>

#include "stack.h"

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

int id_cmp(const void *key, const void *data)
{
      const int *k = key;
      const struct score *d = data;

      return *k - d->id;
}

int main(void)
{
      struct score tmp, *datap;
      STACK *list;
      int i, j, ret, id;

      list = stack_creat(sizeof(tmp));

      for (i = 0; i < 4; i++) {
	    tmp.id = i;
	    tmp.math = 100 - i;
	    snprintf(tmp.name, NAMESIZE, "stu%d", i);

	    ret = stack_push(list, &tmp);
	    if (ret == -1) {
		  break;
	    }
      }
      while (1) {
	    ret = stack_pop(list, &tmp);
	    if (ret == -1) {
		  break;
	    }
	    print_s(&tmp);
      }


      stack_destroy(list);
      return 0;
}
