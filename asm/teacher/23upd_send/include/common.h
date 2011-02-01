#ifndef __COMMON_H_
#define __COMMON_H_

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;


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

/* lib/stdio.c */
extern int getchar(void);
extern int putchar(const char c);
extern int gets(char *s);
extern void puts(const char *s);
extern void printf(const char *fmt, ...);
extern void sprintf(char *buf, const char *fmt, ...);
extern int atoi(const char *cp);
extern void nsleep(unsigned int loops);
extern void usleep(unsigned int loops);
extern void msleep(unsigned int loops);
extern void sleep(unsigned int loops);
extern unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);

/* lib/string.c */
extern char * strcpy(char * dest,const char *src);
extern int strcmp(const char * cs,const char * ct);
extern int strlen(const char * s);
extern int strnlen(const char * s, int count);
extern void * memset(void * s,int c,int count);
extern void * memcpy(void * dest,const void *src,int count);
extern int memcmp(const void * cs,const void * ct,int count);

#endif
