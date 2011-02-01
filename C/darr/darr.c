#include "darr.h"

#define EXTEND 1
#define DELETE 0

void *__realloc(DARR *darr, int flag){
	void *tmp;
	if (darr->num % BUFSTEP == 0){
		tmp = realloc(darr->droot, (darr->num + flag * BUFSTEP) * darr->size);
		return tmp;
	}
	return darr->droot;
}

int save_darr(const DARR *darr, const char *filepath) {
	FILE *fp;
	int n = 0;
	if ((fp = fopen(filepath,"w")) == NULL)
		return -1;
	if (fwrite(&darr->num, sizeof(darr->num), 1, fp) < 1 || fwrite(&darr->size, sizeof(darr->size), 1, fp) < 1)
		return -1;
	while ( n != darr->num)
		n += fwrite(darr->droot + n * darr->size, darr->size, darr->num - n, fp);
	return 0;
}

DARR *load_darr(const char *filepath){
	FILE *fp;
	DARR *darr;
	int n = 0;

	if ((fp = fopen(filepath, "r")) == NULL)
		return NULL;
	if ((darr = malloc(sizeof(*darr))) == NULL) {
		fclose(fp);
		return NULL;
	}
	if (fread(&darr->num, sizeof(darr->num), 1, fp) < 1 || fread(&darr->size, sizeof(darr->size), 1, fp) < 1) {
		free(darr);
		fclose(fp);
		return NULL;
	}
	if ((darr->droot = malloc(darr->num * darr->size)) == NULL) {
		free(darr);
		fclose(fp);
		return NULL;
	}
	while ( n != darr->num)
		n += fread(darr->droot + n * darr->size, darr->size, darr->num - n, fp);
	
	fclose(fp);
	return darr;
}

DARR *create_darr(size_t size) {
	DARR *tmp_darr;

	if ((tmp_darr = malloc(sizeof(*tmp_darr))) == NULL)
		return NULL;
	tmp_darr->num = 0;
	tmp_darr->size = size;
	tmp_darr->droot = NULL;
	
	return tmp_darr;
}

int append_darr(DARR *darr, void *data) {
	void *tmp;
	if ((tmp = __realloc(darr, EXTEND)) == NULL)
		return -1;

	darr->droot = tmp;
	memmove((darr->droot + darr->num * darr->size), data, darr->size);
	darr->num++;
	return 0;
}

void *find_node(DARR *darr, void *key, int (*cmp)(const void *data, const void *key)) {
	int i;	
	if (darr == NULL)
		return;
	for (i = 0; i < darr->num; i++) {
		if (cmp(darr->droot + i * darr->size, key) == 0)
			return darr->droot + i * darr->size; 
	}
	return NULL;
}

//int update_node(void *data, const void *newdata,size_t size) {
//	memcpy(data, newdata, size);
//	return 0; 
//}

void delete_node(DARR *darr, void *key, int(*cmp)(const void *data, const void *key)){
	void *tmp;
	int i;	

	if (darr == NULL)
		return;

	for (i = 0; i < darr->num; i++)
	     if (cmp(darr->droot + i * darr->size, key) == 0) {
			memmove(darr->droot + i * darr->size, darr->droot + (i + 1) * darr->size, \
					(darr->num - i - 1) * darr->size);
			darr->num--;
			if ((tmp = __realloc(darr, DELETE)) != NULL)
			     darr->droot = tmp;
			break;
		}
}

int inserd_node(DARR *darr, const void *data, int i) {
	void *tmp;

	if ((tmp = __realloc(darr, EXTEND)) == NULL)
		return -1;

	if (i < 1)
		i = 1;
	else if (i > darr->num)
		i = darr->num + 1;

	darr->droot = tmp;
	memmove(darr->droot + i * darr->size, darr->droot + (i - 1) * darr->size,\
			 (darr->num - i +1) * darr->size);
	memcpy(darr->droot + (i - 1) * darr->size, data, darr->size);
	darr->num++;

	return 0;
}

void *fetch(DARR *darr, void *key, int(*cmp)(void *data, void *key)){
	void *tmp, *ret;
	int i;

	if (darr == NULL)
		return NULL;

	for (i = 0; i < darr->num; i++)
		if (cmp(darr->droot + i * darr->size, key) == 0) {
			if ((tmp = malloc(darr->size)) == NULL)
				return NULL;
			memcpy(tmp, darr->droot + i * darr->size, darr->size);
			memmove(darr->droot + i * darr->size, darr->droot + (i+1) * darr->size,\
					 (darr->num - i -1) * darr->size);
			darr->num--;
			if ((ret = __realloc(darr, DELETE)) != NULL)
			     darr->droot = ret;
			return tmp;
		}
	
	return NULL;
}

void erase(DARR *darr) {
	free(darr->droot);
	free(darr);
}

void darr_sort(DARR *darr, int(*cmp_fun)(const void *data1, const void *data2)) {
	qsort(darr->droot, darr->num, darr->size, cmp_fun);
}

void travel(DARR *darr, void (*op)(void *data)) {
	int i;
	if (darr == NULL)
		return;
	for (i = 0; i < darr->num; i++)
		op(darr->droot + i * darr->size);
}

