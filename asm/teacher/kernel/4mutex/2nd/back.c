#include <stdio.h>
#include <unistd.h>

int main(void)
{
	for(; ;)
	{
		printf("<app> %s\n", __FUNCTION__);
		sleep(1);	
	}
	return 0;
}
