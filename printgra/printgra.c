#include <stdio.h>
#define LINE 14  /*打印图形大小 print graphic size*/
#define KEY LINE / 2

void enaput(char c){
	putchar(c);
	putchar(' ');
}

int main(void){
	int i = LINE;
	int j = 0;
	int p;

	/*print 1*/
	putchar('\n');
	for(i = 0; i <= LINE - 2; i++){
		for(j = 0; j <= LINE; j++)
			enaput('*');
		putchar('\n');	
	}
	/*print 2*/
	p = 0;
	for(i = 0; i <= KEY; i++){
		for (j = 0; j <= LINE; j++) 
			(j >= KEY - p && j <= KEY + p) ? enaput('*') : enaput(' ');
		p++;
		putchar('\n');	
	}

	/*print 3*/
	putchar('\n');
	p = KEY;
	for(i = 0; i <= KEY; i++){
		for (j = 0; j <= LINE; j++) 
			(j >= KEY - p && j <= KEY + p) ? enaput('*') : enaput(' ');
		p--;
		putchar('\n');	
	}	

	/*print 4*/
	putchar('\n');	
	p = KEY;
	for(i = KEY; i >= 0; i--){
		for (j = 0; j <= LINE + i; j++) 
			(j >= p)? enaput('*') : enaput(' '); 
		p--;
		putchar('\n');	
	}

	/*print 5*/
	putchar('\n');	
	p = KEY;
	for(i = 0; i <= KEY; i++){
		for (j = 0; j <= LINE + i; j++) 
			 (j >= p)? enaput('*') : enaput(' ');  
		p--;
		putchar('\n');	
	}
	return 0;
}
