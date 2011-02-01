#include "myhead.h"

void swap_node_value(struct stu *node1, struct stu *node2) {
	struct stu tmp;

	tmp.id = node2->id;
	strcpy(tmp.name, node2->name);
	tmp.scope = node2->scope;

	node2->id = node1->id;
	strcpy(node2->name, node1->name);
	node2->scope = node1->scope;

	node1->id = tmp.id;
	strcpy(node1->name, tmp.name);
	node1->scope = tmp.scope;

}
void swap_node(struct stu **sturoot, struct stu *node1, struct stu *node2) {
	struct stu *root, *prev_node1, *prev_node2, *next_node1, *next_node2;
	
	if (node1 == node2)
			return;

	root=*sturoot;
	while (root != node1 && root != node2)
		root = root->next;

	if (root == node2) {
		node2 = node1;
		node1 = root;
	}

	root=*sturoot;
	while (root != NULL) {
		if (root->next == node1) {
			prev_node1 = root;
			next_node1 = root->next->next;
		}
		if (root->next == node2) {
			prev_node2 = root;
			next_node2 = root->next->next;
		}
		root = root->next;
	}	

	if (node1 == prev_node2) {
		prev_node1->next = node2;
		node2->next = node1;
		node1->next = next_node2;
	} else {
		prev_node1->next = node2;
		node2->next = next_node1;
		prev_node2->next = node1;
		node1->next = next_node2;
	}

	if (*sturoot == node1)
		*sturoot = node2;
}
/* 添加单元函数 */
int insert(struct stu **sturoot) {
	struct stu *pstu;

	if ( (pstu = read_record()) != NULL) { 	/* 从终端读取函数返回*/
		pstu->next = *sturoot;						/* 从链表头部插入*/
		*sturoot = pstu;
		return 1;
	} 
	return 0;
}

/* 列出链表内容 */
void list(struct stu *pcur) {
	printf("ID\tName\t\tScope\n");
	for (;pcur != NULL; pcur = pcur->next)
		print_node(pcur);
}

/* 保存数据 */
int savedata(struct stu *pcur) {
	int fd;

   if ((fd = open(filename, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)) == -1 )
      return -1;

	for (;pcur != NULL; pcur = pcur->next)
		write(fd, pcur, offsetof(struct stu, next));

	close(fd);

	return 0;
}

/* 加载数据*/
int loaddata(struct stu **sturoot) {
	int fd;
	struct stu *pstu;

   if ((fd = open(filename, O_RDONLY)) == -1)
      return -1;

	while ( (pstu = malloc(sizeof(struct stu))) != NULL && read(fd, (void *)pstu, offsetof(struct stu, next)) != 0) {
		pstu->next = *sturoot;
		*sturoot = pstu;
	}

	close(fd);

	return 0;
}

/*根据条件查找 */
struct stu *find(struct stu *sturoot, const struct stu *tmp){
	struct stu *pcur;
	int id_flag = 0, name_flag = 0, scope_flag = 0;	

	if (tmp->id == -1)
		id_flag = 1;
	if (strlen(tmp->name) == 0)
		name_flag = 1;
	if (tmp->scope == -1)
		scope_flag = 1;
	
	for (pcur = sturoot; pcur != NULL; pcur = pcur->next) 
		if ((id_flag || pcur->id == tmp->id) && (name_flag || strstr(pcur->name,tmp->name) != NULL) && (scope_flag || pcur->scope == tmp->scope) )
			return pcur;
	return NULL;	
}

/*清空链表*/
void erase(struct stu *tmp) {
	if (tmp->next != NULL)
		erase(tmp->next);
	free(tmp);
}

/* 删除节点 */
int delete(struct stu **sturoot, struct stu *tmp, int delete_all_flag){
	struct stu *pcur, *pprev, *temp;
	int id_flag = 0, name_flag = 0, scope_flag = 0, find_it = 0;	

	if (tmp->id == -1)
		id_flag = 1;
	if (strlen(tmp->name) == 0)
		name_flag = 1;
	if (tmp->scope == -1)
		scope_flag = 1;

	do {
		for (pcur = *sturoot; pcur != NULL;) {
			if ((id_flag || pcur->id == tmp->id) && (name_flag || strstr(pcur->name,tmp->name) != NULL) && (scope_flag || pcur->scope == tmp->scope) ) {
				if (pcur != *sturoot)
					pprev->next = pcur->next;		/* 删除并释放*/
				else 
					*sturoot = pcur->next;			/* 如果需要释放的是首元素 */
				temp = pcur->next;
				free(pcur);
				pcur = temp;
				find_it = 1;
				break;
			}
		pprev = pcur;
		pcur = pcur->next; 
		}
	} while (delete_all_flag &&  pcur != NULL);

	return find_it;
}

int update(struct stu *cur, const struct stu *new) {

		if (new->id != -1)
			cur->id = new->id;
		if (strlen(new->name) != 0)
			strcpy(cur->name, new->name);
		if (new->scope != -1)
			cur->scope = new->scope;
		
		return 1;
}
