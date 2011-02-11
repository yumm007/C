#include <stdio.h>

#include "queue.h"

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
      QUEUE *list;
      int i, j, ret, id;

      list = queue_creat(sizeof(tmp));

      for (i = 0; i < 4; i++) {
	    tmp.id = i;
	    tmp.math = 100 - i;
	    snprintf(tmp.name, NAMESIZE, "stu%d", i);

	    ret = queue_en(list, &tmp);
	    if (ret == -1) {
		  break;
	    }
      }
      while (1) {
	    ret = queue_de(list, &tmp);
	    if (ret == -1) {
		  break;
	    }
	    print_s(&tmp);
      }


      queue_destroy(list);
      return 0;
}
