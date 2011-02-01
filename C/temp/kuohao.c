#include <stdio.h>

int check(const char *line) {
	char *p, *tmp;
	int c;

	p = tmp = (char *)line;
	
	while ((c = *tmp++) != '\0')
		if (c == '(') 
			p++;
		else if (c == ')') 
			p--;

	return (p == line ? 0: -1);
}

int check_2(void) {
	int p[4] = {0,0,0,0};
	int i;
	
	for (i = 1; i <5 ; i++) {
		p[0] = (i == 2 ? 1: 0);
		p[1] = (i == 4 ? 1: 0);
		p[2] = (i != 3 ? 1: 0);
		p[3] = (p[1] == 0 ? 1: 0);
		
		if(p[0] + p[1] + p[2] + p[3] == 1)
			return i;
	}
	
	return 0;
}

int main(void)
{
	char temp1[] = "1+2*(3-2)*(6-1)";
	char temp2[] = ")1+(2*(3-2)*(6-1))";
	char temp3[] = "(1+2)*((3-4)/5)";


	if (check(temp3) == 0)
		printf("YES!\n");
	else 
		printf("NO!\n");

	printf("%d\n", check_2());

	return 0;
}
