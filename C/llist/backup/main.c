#include "myhead.h"


int main(void)
{
	LLIST *sturoot;

   if ((sturoot = load_llist("db.db")) == NULL){
      printf("load db error.");
      any_key();
   }

   
	travel(sturoot, print_node);
	printf("--------------\n");

             struct stu *tmp, *find;
               if ((tmp = calloc(sizeof(*tmp) + MAXNAME,sizeof(char))) == NULL) 
                  printf("malloc error!\n");
               else if (getline(tmp) == 0)  
                  printf("Input Format wrong!\n");
               else if ((tmp = realloc(tmp, offsetof(struct stu, name) + strlen(tmp->name) + 1)) == NULL)
                  printf("realloc error!\n");
               else if ((find = fetch(sturoot, tmp, cmp, NEXT)) == NULL)
                  printf("Record is not exist!");
               else
                  print_node(find);
               free(tmp);



	printf("--------------\n");
	travel(sturoot, print_node);
	
	return 0;
}
