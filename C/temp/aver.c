#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static char line[1000];

int average(int n_values, ...) {

	va_list var_arg;
	int count;
	float sum = 0;

	va_start(var_arg, n_values);

	for (count = 0; count < n_values; count += 1)
			sum += va_arg(var_arg, int);

	va_end(var_arg);

	return sum / n_values;
}

void megstr(char *dest, ...) {
	
	va_list ap;

	va_start(ap, dest);
	
	while (*(char **)ap != NULL) {
		strcat(dest, (char *)va_arg(ap, char **));
	}

	va_end(ap);
}

int main(void)
{

	int n;

	n = average(3, 8, 9, 6, 20);
	
	megstr(line, "line1\t", "line desk ok!","line2\t","line3\t", NULL);

	printf("line = %s\n",line);

	return 0;
}
