#ifndef	PTH_QSORT_H
#define	PTH_QSORT_H

#define	PTH_NUM	4

#ifndef	STACK_SIZE
#define 	STACK_SIZE	1024
#endif

#define 	EMALLOC	-1

typedef int cmp_fun(const void *data1, const void *data2);
struct sort_data{
	void *base;
	int left;
	int right;
	size_t size;
	cmp_fun *cmp;
};

int pth_sort(struct sort_data *data);

#endif

