#ifndef __COMMON_H_
#define __COMMON_H_

#define NULL ((void *)0)

#define setbit(addr, bit, shift) (*((unsigned int *)addr) |= ((bit) << (shift)))
#define clrbit(addr, bit, shift) (*((unsigned int *)addr) &= ~((bit) << (shift)))

#define setval(addr, val, count, shift) 			\
	{						\
		clrbit(addr, ~(~0 << count), shift);	\
		setbit(addr, val, shift);		\
	}

#endif

