#include <stdio.h>

#include <llist.h>

#define NAMESIZE 32
#define LISTNUM 127

struct score {
      int id;
      char name[NAMESIZE];
      int math;
};

LLIST *hlist[LISTNUM];

int hlist_init(void)
{
      int i;

      for (i = 0; i < LISTNUM; i++) {
	    hlist[i] = llist_creat(sizeof(struct score));
	    /* if error */
      }

      return 0;
}

void print_s(struct score *data)
{
      printf("%d, %s, %d\n", data->id, data->name, data->math);
}

unsigned int hash(const char *key)
{
      unsigned int sum = 0;

      for (; *key != '\0'; key++) {
	    sum = (sum << 5) ^ *key;
      }

      return sum;
}

int insert(struct score *data)
{
      int ind;

      ind = hash(data->name) % LISTNUM;
      return llist_insert(hlist[ind], data, LLIST_FORWARD);
}

static int name_cmp(const void *name, const void *data)
{
      const char *n = name;
      const struct score *d = data;

      return strcmp(n, d->name);
}

struct score *find(const char *name)
{
      int ind;

      ind = hash(name) % LISTNUM;
      return llist_find(hlist[ind], name, name_cmp, LLIST_FORWARD);
}

void travel(void)
{
      int i;

      for (i = 0; i < LISTNUM; i++) {
	    printf("%d ", llist_getnum(hlist[i]));
      }
      printf("\n");
}

void rand_name(char *buf, int size)
{
      int len;
      int i;

      len = rand() % (size - 1) + 1;
      for (i = 0; i < len; i++) {
	    buf[i] = rand() % 26 + 'a';
      }
      buf[i] = '\0';
}

int main(void)
{
      struct score tmp, *datap;
      int i;

      srand(getpid());
      hlist_init();
      for (i = 0; i < 4096; i++) {
	    tmp.id = i;
	    tmp.math = 100 - i;
	    //snprintf(tmp.name, NAMESIZE, "stu%d", i);
	    rand_name(tmp.name, NAMESIZE);

	    insert(&tmp);
      }

#if 0
      datap = find("stu4");
      if (datap == NULL) {
	    printf("Can not find.\n");
      } else {
	    print_s(datap);
      }
#endif
      travel();

      return 0;
}
