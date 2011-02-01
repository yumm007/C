#ifndef __COMMON_H_
#define __COMMON_H_

#include <up2440.h>

typedef unsigned long 	u32;
typedef unsigned short 	u16;
typedef unsigned char 	u8;

#define NULL ((void *)0)

#define set1(var, shift) ((var) |= (1 << (shift)))
#define set0(var, shift) ((var) &= ~(1 << (shift)))

#define setbit(var, bit, shift) (var |= ((bit) << (shift)))
#define clrbit(var, bit, shift) (var &= ~((bit) << (shift)))

#define setval(var, val, count, shift) 			\
	{						\
		clrbit(var, ~(~0 << count), shift);	\
		setbit(var, val, shift);		\
	}

/* kernel/shell.c */
extern void shell(void);
extern int exec_command(const char *cmd);

/* kernel/cpu/start.S */
extern void exit(void);

/* lib/stdio.c */
extern int getchar(void);
extern int putchar(const char c);
extern int gets(char *s);
extern void puts(const char *s);
extern void printf(const char *fmt, ...);
extern void sprintf(char *buf, const char *fmt, ...);
extern unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);
extern long simple_strtol(const char *cp,char **endp,unsigned int base);
extern int ustrtoul(const char *cp, char **endp, unsigned int base);
extern void nsleep(unsigned int loops);
extern void usleep(unsigned int loops);
extern void msleep(unsigned int loops);
extern void sleep(unsigned int loops);
extern int atoi(const char *cp);
extern int atoh(const char *cp);

/* lib/string.c */
extern char * strcpy(char * dest,const char *src);
extern char * strncpy(char * dest,const char *src,int count);
extern char * strcat(char * dest, const char * src);
extern char * strncat(char *dest, const char *src, int count);
extern int strcmp(const char * cs,const char * ct);
extern int strncmp(const char * cs,const char * ct,int count);
extern int strlen(const char * s);
extern int strnlen(const char * s, int count);
extern char * strchr(const char * s, int c);
extern char * strrchr(const char * s, int c);
extern char * strdup(const char *s);
extern int strspn(const char *s, const char *accept);
extern char * strpbrk(const char * cs,const char * ct);
extern char * strtok(char * s,const char * ct);
extern char * strsep(char **s, const char *ct);
extern char * strswab(const char *s);
extern void * memset(void * s,int c,int count);
extern char * bcopy(const char * src, char * dest, int count);
extern void * memcpy(void * dest,const void *src,int count);
extern void * memmove(void * dest,const void *src,int count);
extern int memcmp(const void * cs,const void * ct,int count);
extern void * memscan(void * addr, int c, int size);
extern char * strstr(const char * s1,const char * s2);
extern void * memchr(const void *s, int c, int n);

#endif
