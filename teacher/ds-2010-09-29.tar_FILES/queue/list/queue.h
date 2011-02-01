#ifndef HS_STACK_H
#define HS_STACK_H

#include <llist.h>

typedef LLIST QUEUE;

QUEUE *queue_creat(int size);

void queue_destroy(QUEUE *);

int queue_en(QUEUE *, void *data);

int queue_de(QUEUE *, void *data);


#endif
