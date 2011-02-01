#ifndef __COMMON_H_
#define __COMMON_H_

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

#endif

