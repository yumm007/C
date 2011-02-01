#include <stdio.h>
#define YEAR 2008
#define Yes (YEAR % 4 == 0 && YEAR % 100 != 0 || YEAR % 400 == 0)

int main(void){
	
	enum {Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Dec, Nov};
	
	int days[]={31,28+Yes,31,30,31,30,31,31,30,31,30,31};
	
	printf("Feb = %d\n", days[Feb]);

	return 0;	
	
}
