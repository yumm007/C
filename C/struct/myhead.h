/*
**  学生成绩管理系统
**	 2010-09-15 yumm_007@163.com
*/


#ifndef _PRINTSTU_H
#define _PRINTSTU_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>

#define MAXNAME 20
#define DELETE_ALL_YES 1
#define DELETE_ALL_NO 0
#define SORT_BY_ID 1
#define SORT_BY_NAME 2
#define SORT_BY_SCOPE 3

static const char filename[] = "db.db";

struct stu {
  int id;
  char name[MAXNAME];
  int scope;
  struct stu *next;
};

void any_key(void);
int getline(struct stu *);
void print_node(struct stu *p);
struct stu *read_record(void);
struct stu *find(struct stu *root, const struct stu *tmp);
int get_num(void);
int read_select(void);
int insert(struct stu **sturoot);
void list(struct stu *pcur);
void erase(struct stu *tmp);
int delete(struct stu **sturoot, struct stu *need_del, int delete_all_flag);
int update(struct stu *tmp_from, const struct stu *tmp_to);
int cmp_scope(const void *data1, const void *data2);
int cmp_id(const void *data1, const void *data2);
int cmp_name(const void *data1, const void *data2);
void sort_stu(struct stu **sturoot, int sort_by);
void swap_node(struct stu **sturoot, struct stu *node1, struct stu *node2);
void swap_node_value(struct stu *node1, struct stu *node2);
int savedata(struct stu *);
int loaddata(struct stu **);

#endif

