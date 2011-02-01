#ifndef U_STRING_H
#define U_STRING_H

int str_cmp(const char *a1, const char *b1);
void mem_set(char *buf, int val, int len);
void serial_putc(unsigned char c);
unsigned char serial_getc(void);

#endif 
