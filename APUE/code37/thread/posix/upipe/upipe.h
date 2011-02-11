#ifndef UPIPE_H
#define UPIPE_H

#define UPIPE_ATOMICSIZE 256

typedef void upipe_t;

struct upipe_stat {
	size_t count;
};

upipe_t *upipe_new(size_t pipesize);
int upipe_delete(upipe_t *);

ssize_t upipe_read(upipe_t *, void *, size_t);
ssize_t upipe_write(upipe_t *, const void *, size_t);
ssize_t upipe_stat_unlocked(upipe_t *, struct upipe_stat *res);

#endif
