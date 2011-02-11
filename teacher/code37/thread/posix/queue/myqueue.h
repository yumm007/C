#ifndef MYQUEUE_H
#define MYQUEUE_H

typedef void queue_t;

queue_t *queue_init(void);
int queue_init(queue_t *);

ssize_t queue_write(queue_t *, const void*);
ssize_t queue_read(queue_t *, void**);

#endif

