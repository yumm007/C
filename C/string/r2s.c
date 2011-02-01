#include <stdio.h>
#define PI 3.1415926ul
#define START 55
#define END 60
#define STEP 0.5

double ctos(double r){

	double res;

	res = PI * r * r;

	return res;
}

int main(void){
	
	double r, s;
	
	for (r = START; r < END + STEP; r = r + STEP){
		s = ctos(r);
		printf("r=%.1f\tS=%f\n", r, s);
	}

#if 0
	for (r = START; r < END + STEP; r = r + STEP){
		s = ctos(r);
		printf("r=%.1f\tS=%f\n", r, s);
	}
#endif

	return 0;
}
