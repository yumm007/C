#include <stdio.h>
#include <stdlib.h>

#include "list.h"

#define NAMESIZE 32
struct score {
      int id;
      char name[NAMESIZE];
      int math;
      struct list_head node;
};

void print_s(void *data)
{
      struct score *d = data;

      printf("%d, %s, %d\n", d->id, d->name, d->math);
}

int main(void)
{
      struct score *tmp, *datap;
      LIST_HEAD(head);
      struct list_head *cur;
      int ret, i, j;

      for (j = 0; j < 2; j++) {
	    for (i = 0; i < 4; i++) {
		  tmp = malloc(sizeof(*tmp));
		  if (tmp == NULL) {
			break;
		  }

		  tmp->id = i;
		  tmp->math = 100 - (i + j);
		  snprintf(tmp->name, NAMESIZE, "stu%d", i + j);

		  list_add(&tmp->node, &head);
	    }
      }

      __list_for_each(cur, &head) {
	    datap = list_entry(cur, struct score, node);
	    print_s(datap);
      }
      printf("\n");

      __list_for_each(cur, &head) {
	    datap = list_entry(cur, struct score, node);
	    if (datap->id == 2) {
		  __list_del(cur->prev, cur->next);
		  free(datap);
		  break;
	    }
      }

      __list_for_each(cur, &head) {
	    datap = list_entry(cur, struct score, node);
	    print_s(datap);
      }

#if 0
      id = 2;
      datap = llist_find(list, &id, id_cmp, LLIST_BACKWARD);
      if (datap == NULL) {
	    printf("Can not find.\n");
      } else {
	    print_s(datap);
      }
      id = 3;
      llist_delet(list, &id, id_cmp, LLIST_FORWARD);
      llist_travel(list, print_s);

      llist_destroy(list);
#endif

      return 0;
}
