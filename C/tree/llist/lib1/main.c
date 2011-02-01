#include <stdio.h>

#include "llist.h"

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
      LLIST *list;
      int i, j, ret, id;

      list = llist_creat(sizeof(tmp));

      for (j = 0; j < 2; j++) {
	    for (i = 0; i < 4; i++) {
		  tmp.id = i;
		  tmp.math = 100 - (i + j);
		  snprintf(tmp.name, NAMESIZE, "stu%d", i + j);

		  ret = llist_insert(list, &tmp, LLIST_FORWARD);
		  if (ret == -1) {
			break;
		  }
	    }
      }

      llist_travel(list, print_s);
      printf("\n");

#if 0
      id = 2;
      datap = llist_find(list, &id, id_cmp, LLIST_BACKWARD);
      if (datap == NULL) {
	    printf("Can not find.\n");
      } else {
	    print_s(datap);
      }
#endif

      id = 3;
      llist_delet(list, &id, id_cmp, LLIST_FORWARD);
      llist_travel(list, print_s);

      llist_destroy(list);
      return 0;
}
