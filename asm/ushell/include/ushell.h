#ifndef U_SHELL_H
#define U_SHELL_H

#define getchar	serial_getc
#define putchar	serial_putc

typedef int (*FUN)(int argc, char **argv);

struct buidin_fun {
	char 	*name;
	FUN	fun;
};

extern void init_com(void);
extern void puts(const char *s);
extern void gets(char *s);

#endif
