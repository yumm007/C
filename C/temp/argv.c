#include <stdio.h>

int main(int argc, char *argv[])
{

	while (*++argv != NULL && **argv == '-') {
		while (*++*argv != '\0')
			switch (**argv) {
			case 'a':
				printf("option is a.\n");	
				break;
			case 'b':
				printf("option is b.\n");	
				break;
			case 'c':
				printf("option is c.\n");	
				break;
			case 'd':
				printf("option is d.\n");	
				break;
			default:
				break;
			}
	}

	return 0;
}
