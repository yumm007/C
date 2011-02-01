#include "klist.h"

#define NAMESIZE 32
struct score {
      int id;
      int math;
      char name[NAMESIZE];
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

      return d->id - *k;
}

int main(void)
{
      struct score tmp, *datap;
      LLIST *list;
      int i, ret, id;

      //list = create_llist();
/*
	    for (i = 0; i < 8; i++) {
		  tmp.id = i;
		  tmp.math = 100 - i;
		  snprintf(tmp.name, NAMESIZE, "stu%d", i);
		  ret = insert_llist(list, &tmp, offsetof(struct score, name) + strlen(tmp.name) + 1 ,NEXT);
		  if (ret == -1) {
			break;
		  }
	    }
*/
		list = load_llist("db.db");

      travel(list, print_s);
		i = 2;
		printf("-------find 2----------\n");
		print_s(find_node(list,&i,id_cmp,NEXT));
		printf("-------delete 7----------\n");
		i = 7;
		delete_node(list,&i,id_cmp,NEXT);
      travel(list, print_s);
		i = 3;
		printf("------fetch 3-----------\n");
		print_s(fetch(list,&i,id_cmp,NEXT));
		printf("-----------------\n");
      travel(list, print_s);
		printf("--------sort---------\n");
      llist_sort(list, id_cmp, print_s);
		
      printf("\n");

		//save_llist(list, "db.db");


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
