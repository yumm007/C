#ifndef _HS_MYHEARD_H
#define _HS_MYHEARD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "klist.h"

#define MAXNAME 20

struct stu {
      int id;
      int scope;
      char name[1];
};

void print_node(void *data);
void any_key(void);
int getline(struct stu *tmp);
void print_node(void *data);
struct stu *read_record(void);
int get_num(void);
int cmp(const void * data, const void *key);
//int update_node(struct stu *cur, const struct stu *new);
int cmp_id(const void *data1, const void *data2);
int cmp_scope(const void *data1, const void *data2);
int cmp_name(const void *data1, const void *data2);

#endif
