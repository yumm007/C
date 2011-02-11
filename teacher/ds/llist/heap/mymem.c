#include <stddef.h>

#include "mymem.h"

static void *align_up(void *ptr)
{
      unsigned long p = (unsigned long)ptr;

      return (void *)((p + MYMEM_ALIGN - 1) / MYMEM_ALIGN * MYMEM_ALIGN);
}

static void *align_down(void *ptr)
{
      unsigned long p = (unsigned long)ptr;

      return (void *)(p / MYMEM_ALIGN * MYMEM_ALIGN);
}

MEM *mymem_init(void *start, unsigned size)
{
      void *end;
      MEM *new;

      end = (char *)start + size;
      start = align_up(start);
      end = align_down(end);

      new = start;
      new->prev = new->next = new + 1;
      new->end = (char *)(new + 1);

      new[1].prev = new[1].next = new;
      new[1].end = end;

      return new;
}

void mymem_clean(MEM *ptr)
{
      return;
}

static unsigned realsize(unsigned size)
{
      size = (unsigned)align_up((void *)size);

      /* fix me, align */
      size += sizeof(void *);
      size = size > sizeof(MEM) ? size : sizeof(MEM);

      return size;
}

static unsigned nodesize(MEM *node)
{
      return node->end - (char *)node;
}

#if 0
static MEM *find_node(MEM *ptr, unsigned size)
{
      MEM *cur;

      for (cur = ptr->next; cur != ptr; cur = cur->next) {
	    if (size <= nodesize(cur)) {
		  return cur;
	    }
      }

      return NULL;
}
#else
static MEM *find_node(MEM *ptr, unsigned size)
{
      MEM *cur;
      MEM *min = NULL;
      unsigned minsize, cursize;

      for (cur = ptr->next; cur != ptr; cur = cur->next) {
	    cursize = nodesize(cur);
	    if (size <= cursize) {
		  if (min == NULL || cursize < minsize) {
			min = cur;
			minsize = cursize;
		  }
	    }
      }

      return min;
}
#endif

static void *mem2usr(void *start, void *end)
{
      /* fix me, align */
      void **s = start;

      *s = end;
      return s + 1;

      s[0] = end;
      return &s[1];
}

static void *node2usr(MEM *ptr, MEM *node, unsigned size)
{
      void *start, *end;

      if (node == NULL) {
	    return NULL;
      }

      end = node->end;
      if (nodesize(node) - size < MYMEM_REMAIN) {
	    node->next->prev = node->prev;
	    node->prev->next = node->next;

	    start = node;
      } else {
	    node->end -= size;
	    start = node->end;
      }

      return mem2usr(start, end);
}

void *mymem_alloc(MEM *ptr, unsigned size)
{
      MEM *node;

      size = realsize(size);

      node = find_node(ptr, size);
      return node2usr(ptr, node, size);
}

static MEM *mem2node(void *mem)
{
      MEM *node;
      void **start;

      /* fix me, align */
      start = (void **)mem - 1;
      node = (MEM *)start;

      node->end = *start;
      return node;
}

static void __insert_node(MEM *node, MEM *prev, MEM *next)
{
      node->next = next;
      node->prev = prev;
      node->next->prev = node;
      node->prev->next = node;
}

static void mergenode(MEM *ptr, MEM *node)
{
      if (node->end == (char *)node->next) {
	    node->end = node->next->end;
	    node->next = node->next->next;
	    node->next->prev = node;
      }

      if ((char *)node == node->prev->end && node->prev != ptr) {
	    node = node->prev;
	    node->end = node->next->end;
            node->next = node->next->next;
            node->next->prev = node;
      }
}

#if 0
static void mymem_insert(MEM *ptr, MEM *node)
{
      __insert_node(node, ptr, ptr->next);
}
#else
static void mymem_insert(MEM *ptr, MEM *node)
{
      MEM *cur;

      for (cur = ptr->next; cur != ptr; cur = cur->next) {
	    if (cur > node) {
		  break;
	    }
      }

      __insert_node(node, cur->prev, cur);
      mergenode(ptr, node);
}
#endif

void mymem_free(MEM *ptr, void *mem)
{
      MEM *newnode;

      newnode = mem2node(mem);
      mymem_insert(ptr, newnode);
}
