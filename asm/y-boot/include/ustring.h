#ifndef U_STRING_H
#define U_STRING_H

int str_cmp(const char *a1, const char *b1);
void mem_set(char *buf, int val, int len);
void mem_cpy(char *dest, char *sour, int len);
void serial_putc(unsigned char c);
unsigned char serial_getc(void);
int atoi(const char *cp);
int atoh(const char *cp);
int printf(const char *format, ...);

#endif 
