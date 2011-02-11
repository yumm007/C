#include <stddef.h>

#include "queue.h"

QUEUE *queue_creat(int size)
{
      return llist_creat(size);
}

void queue_destroy(QUEUE *ptr)
{
      llist_destroy(ptr);
}

int queue_en(QUEUE *ptr, void *data)
{
      return llist_insert(ptr, data, LLIST_FORWARD);
}

static int always_match(const void *p1, const void *p2)
{
      return 0;
}

int queue_de(QUEUE *ptr, void *data)
{
      return llist_fetch(ptr, data, NULL, always_match, LLIST_BACKWARD);
}
