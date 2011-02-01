#ifndef HASH_LINK_H
#define HASH_LINK_H

typedef int (*cmp_fun)(const void *data, const void *key);
typedef void HASHLINK;

typedef struct {
	void 		*key;
	size_t 	key_size;
	void 		*data;
	size_t 	data_size;
} HL_DATA;

HASHLINK *link_init(size_t hask_size, cmp_fun fun);
void link_dstr(HASHLINK *link);
int  link_add_node(const HASHLINK *link,const HL_DATA *data);
/*返回0时，把数据指针赋给结构体里的data，长度写在data_size*/
int  link_fnd_node(const HASHLINK *link, HL_DATA *data);
int  link_del_node(const HASHLINK *link, HL_DATA *data);

#endif

