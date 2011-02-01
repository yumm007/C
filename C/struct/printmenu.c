#include "myhead.h"

/* 刷屏和读取选项  */
int read_select(void) {			
	int c;

	system("clear");

	printf("**********************************************\n");
	printf("*      Math Class Stystem                    *\n");
	printf("*         1)  Find                           *\n");
	printf("*         2)  Insert                         *\n");
	printf("*         3)  Delete                         *\n");
	printf("*         4)  List                           *\n");
	printf("*         5)  Erase All                      *\n");
	printf("*         6)  Delete duplication             *\n");
	printf("*         7)  Update                         *\n");
	printf("*         8)  Sort                           *\n");
	printf("*         9)  Save to file                   *\n");
 //printf("*         @)  Load from file                 *\n");
	printf("*         0)  Exit                           *\n");
	printf("**********************************************\n");
	printf("\nPlease enter your choose:");

	c = getchar();	/* 这两行用于使程序暂停,获取选项后程序返回 */
	any_key();
	return c;
}

int main(void) 
{
	
	int i;
	struct stu *sturoot = NULL;	/* 链表首地址指针 */	

	if (loaddata(&sturoot) == -1)
		fprintf(stderr, "load file failed.\n");

	/* 
	** 0,q for exit; 1,f for find; 2, i for insert; 3,d for delete, 4, l for list, 5, e for erase;
	** 6, a for delete duplication; 7, u for update, 8, s for sort, 9 for Save data, @ for Load data;
	*/
	while ((i = read_select()) != '0' && i != 'q' && i != 'Q')			/* 主循环,获取控制输入*/
		switch (i){
		case '1': case 'F': case 'f':
				printf("Please enter the search condition: [ID],[Name],[Scope]\n");
				{
					struct stu tmp, *ret;
					if (getline(&tmp) == 0) {
						printf("Input Format wrong!\n");
					} else if ((ret = find(sturoot,&tmp)) == NULL)
						printf("Record is not exist!");
					else
						print_node(ret);
				}
				any_key();
				break;
		case '2': case 'I': case 'i':
				printf("Please enter the new cord: 'ID,Name,Scope:'\n");
				if (insert(&sturoot) == 0)
					printf("Input Error!");
				else
					printf("Insert OK!");
				any_key();
				break;
		case '3': case 'D': case 'd':
				printf("Please enter the condition : [ID],[Name],[Scope]\n");
				{
					struct stu tmp;
					if (getline(&tmp) == 0) 
						printf("Input Format wrong!");
					else if (delete(&sturoot,&tmp, DELETE_ALL_NO) == 0) 
						printf("Record is not exist!");
					else 
						printf("Delete succeed!");
				}
				any_key();
				break;
		case '4': case 'L': case 'l':
				list(sturoot);
				any_key();
				break;
		case '5': case 'E': case 'e':
				erase(sturoot);
				printf("All data has erase!!");
				sturoot = NULL;
				any_key();
				break;
		case '6': case 'A': case 'a':
				printf("Please enter the condition: [ID],[Name],[Scope]\n");
				{
					struct stu tmp;
					if (getline(&tmp) == 0) {
						printf("Input Format wrong!");
					} else if (delete(&sturoot, &tmp, DELETE_ALL_YES) == 0)
						printf("Record is not exist!");
				}
				any_key();
				break;
		case '7': case 'U': case 'u':
				{
					struct stu tmp_from, tmp_to, *tmp_find;
					printf("Please enter the condition: [ID],[Name],[Scope]\n");
					if (getline(&tmp_from) == 0) {
						printf("Input Format wrong!");
					} else if ((tmp_find = find(sturoot,&tmp_from)) != NULL) {
						print_node(tmp_find);
						printf("Please enter the new data: [ID],[Name],[Scope]\n");
						if (getline(&tmp_to) == 0) 
							printf("Input Format wrong!");
						else if (update(tmp_find, &tmp_to) == 1)
							printf("Update Succeed.");
					} else 
							printf("Record is not exist!");
				}
				any_key();
				break;
		case '8': case 'S': case 's':
				{
					int sel;
					printf("Please select sort by:\n1[by ID]\t2[by name]\t3[by scope]:");
					if ((sel=get_num()) <= 3 && sel >= 1) {
						sort_stu(&sturoot,sel);
						printf("Sort Succeed.");
					} else
						printf("Select wrong.");
				}
				any_key();
				break;
		case '9':
				if (savedata(sturoot) == -1)
					fprintf(stderr, "Open file failed.\n");
				any_key();
				break;
/*
		case '@':
				if (loaddata(&sturoot) == -1)
					fprintf(stderr, "Open file failed.\n");
				any_key();
				break;
*/
		default:
				break;
		}

	return 0;
}
