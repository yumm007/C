#include "queue.h"

#define QUEUE_NUM 6

static int queue[QUEUE_NUM];
static int en_ind, de_ind;

static int __isempty(void)
{
      if (en_ind == de_ind) {
	    return 1;
      }
      return 0;
}

static int __isfull(void)
{
      if (de_ind - en_ind == 1 || en_ind == QUEUE_NUM - 1 && de_ind == 0) {
	    return 1;
      }
      return 0;
}

int queue_en(int data)
{
      if (__isfull()) {
	    return -1;
      }

      queue[en_ind++] = data;
      if (en_ind == QUEUE_NUM) {
	    en_ind = 0;
      }
      return 0;
}

int queue_de(int *data)
{
      if (__isempty()) {
	    return -1;
      }

      *data = queue[de_ind++];
      if (de_ind == QUEUE_NUM) {
	    de_ind = 0;
      }
      return 0;
}
