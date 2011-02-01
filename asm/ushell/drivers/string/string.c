
int str_cmp(const char *a1, const char *b1) {
   char *a = (char *)a1, *b = (char *)b1; 

   while (*a == *b && *a != '\0' && *b != '\0') {
      a++;
      b++;
   }   

   return *a - *b; 
}

void mem_set(char *buf, int val, int len) {
	int i;
	for (i = 0; i < len; i++)
      buf[i] = val;
}

