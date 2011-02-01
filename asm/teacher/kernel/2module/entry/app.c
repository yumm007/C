#include <stdio.h>

int main(void)
{
	printf("hello app\n");
	printf("[%s][%d][%s]\n", __FILE__, __LINE__, __FUNCTION__);
	return 0;
}
