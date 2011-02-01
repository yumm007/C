#include "myhead.h"

void any_key(void) {	
     while(getchar() != '\n')						/* 暂停功能 */
	  ;
}

int getline(struct stu *tmp) {
	int c, flag = 1, i = 0, id_flag = 0, scope_flag = 0;

	tmp->id = 0;
	tmp->scope = 0;

   while ((c = getchar()) != '\n') {
	  if (c >= '0' && c <= '9' && flag == 1) {    /* 读取ID*/
	       tmp->id = tmp->id * 10 + c -'0';
			 id_flag = 1;
	  } else if (c == ',')	/*下一个字符属于下一个元素*/
	       flag++;
	  else if (flag == 2 && i < MAXNAME)  /*flag == 2 则读取name*/
	       (tmp->name)[i++] = c;
	  else if (flag == 3 && c >= '0' && c <= '9') { /* ==3 读取scope*/
	       tmp->scope = tmp->scope * 10 + c -'0';
			 scope_flag = 1;
	  }
  	}
	  

     if (flag != 3)
	  return 0;

	  if (id_flag == 0)
			tmp->id = -1;
	  if (scope_flag == 0)
			tmp->scope = -1;

     (tmp->name)[i] = '\0';

     return 1;
}

/* 打印某节点内容 */
void print_node(void *data){
	struct stu *p = (struct stu *)data;
     printf("%d\t%s\t\t%d\n",p->id, p->name, p->scope);
}

/* 读取输入并以此初始化结构体单元 */
struct stu *read_record(void){
     struct stu *pstu = malloc(sizeof(struct stu)); /* 申请空间并初始化*/
     if (getline(pstu) == 0) {
	  		free(pstu);
	  		return NULL;
     }
     return pstu;
}

int update_node(struct stu *cur, const struct stu *new) {

		if (new->id != -1)
			cur->id = new->id;
		if (strlen(new->name) != 0)
			strcpy(cur->name, new->name);
		if (new->scope != -1)
			cur->scope = new->scope;
		
		return 1;
}

int get_num(void) {
     int key = 0, c;
     while ((c = getchar()) != '\n')
	  if (c >= '0' && c <= '9')
	       key = key * 10 + c - '0';
     return key;
}

int cmp_id(const void *data1, const void *data2) {
	return ( ((struct stu *)data1)->id - ((struct stu *)data2)->id );
}

int cmp_scope(const void *data1, const void *data2) {
	return ( ((struct stu *)data1)->scope - ((struct stu *)data2)->scope );
}

int cmp_name(const void *data1, const void *data2) {
	return ( strcmp(((struct stu *)data1)->name, ((struct stu *)data2)->name) );
}

#if 0
void sort_stu(struct stu **sturoot, int sort_by) {
	int (* cmp_fun)(const void *data1, const void *data2);
	struct stu *i, *j; 
	
	if (sort_by == SORT_BY_ID)
		cmp_fun = cmp_id;
	else if (sort_by == SORT_BY_NAME)
		cmp_fun = cmp_name;
	else if (sort_by == SORT_BY_SCOPE)
		cmp_fun = cmp_scope;

	for (i = *sturoot; i != NULL; i = i->next) 
		for (j = *sturoot; j != NULL && j->next != NULL; j = j->next) 
			if (cmp_fun(j, j->next) > 0 ) 
				//swap_node(sturoot,j,j->next);
				swap_node_value(j,j->next);
}

#endif

/*根据条件查找 */
//struct stu *find(struct stu *sturoot, const struct stu *tmp){
int cmp(const void * data, const void *key){
	struct stu *pcur = (struct stu *)data;
	struct stu *tmp = (struct stu *)key;
	int id_flag = 0, name_flag = 0, scope_flag = 0;	

	if (tmp->id == -1)
		id_flag = 1;
	if (strlen(tmp->name) == 0)
		name_flag = 1;
	if (tmp->scope == -1)
		scope_flag = 1;
	if ((id_flag || pcur->id == tmp->id) && (name_flag || strstr(pcur->name,tmp->name) != NULL) && (scope_flag || pcur->scope == tmp->scope) )
			return 0;
	return -1;	
}

