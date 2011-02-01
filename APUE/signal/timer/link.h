#ifndef	MY_LINK_H
#define	MY_LINK_H

typedef int (*cmp_fun)(const void *data1, const void *data2);
typedef void (*op_fun)(const void *data);
typedef int (*proc_fun)(void *data);

void *link_init(size_t size, cmp_fun ins_fun, cmp_fun del_fun, proc_fun emp_fun);
void link_erase(void *head);
int link_add(const void *node, void *head);
int link_del(const void *node, void *head);

/*	link_each 返回链表的每个元素, node为NULL时从head开始遍历 */
//void *link_each(const void *head, const void *node);
void *link_each(const void *head, void **node);
void link_travel(const void *head, op_fun fun);

#endif

