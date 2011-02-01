
typedef unsigned int u32;

void (*printf)(const char *, ...) = (void *)0x33f9291c;

void memset(char *p, char val, u32 length)
{
	u32 i;
	for(i = 0; i < length; i++)
		p[i] = val;
}

void strcpy(char *p1, char *p2)
{
	while(*p2)
		*p1++ = *p2++;
	*p1 = 0;
}



void *malloc(u32 len)
{
	static unsigned char *base = (void *)0x31000000;

	unsigned char *tmp = base;

	base += len;
	
	memset(tmp, 0, len);

	return tmp;
}

void small_map(u32 *ttb, u32 va_start, u32 va_end, u32 pa)
{
	u32 tmp = va_end;
	va_start >>= 12;
	va_end >>= 12;
	if(tmp & 0xfff)
		va_end += 1;

	while(va_start < va_end)
	{
		u32 *ttb2 = malloc(4 * 256);
		
		ttb[va_start >> 8] = ((u32)ttb2 & 0xfffffc00) | 0x11;

		tmp = (va_start | 0xff) + 1;

		if(tmp > va_end)
			tmp = va_end;

		while(va_start < tmp)
		{
			ttb2[va_start & 0xff] = (pa & 0xfffff000) | 0x2;

			va_start += 1;
			pa += 0x1000;
		}
	}
}

void enable_mmu(u32 *ttb)
{
	__asm__ __volatile__ (
		"mcr	p15, 0, %0, c2, c0, 0\n\t"
	
		"mov	r0, #0\n\t"
		"mcr	p15,0,r0,c8,c7,0\n\t"	

		"mrc	p15, 0, r0, c1, c0, 0\n\t"
		"orr	r0, r0, #1\n\t"
		"mcr	p15, 0, r0, c1, c0, 0\n\t"

		::"r"(ttb)
		:"r0"
	);
}

void disable_mmu(void)
{
	__asm__ __volatile__ (
		"mrc	p15, 0, r0, c1, c0, 0\n\t"
		"bic	r0, r0, #1\n\t"
		"mcr	p15, 0, r0, c1, c0, 0\n\t"
		:::"r0"
	);
}

int main(void)
{
	u32 *ttb = (void *)0x30100000;

	memset((char *)ttb, 0, 4 * 4096);

	small_map(ttb, 0, 4096, 0);
	small_map(ttb, 0x30000000, 0x34000000, 0x30000000);
	small_map(ttb, 0x48000000, 0x60000000, 0x48000000);

	strcpy((void *)0x32000000, "hello mmu....");

	small_map(ttb, 0x80000000, 0x80001000, 0x32000000);
	small_map(ttb, 0x90000000, 0x90001000, 0x32000000);

	enable_mmu(ttb);

	printf("%s\n", 0x80000000);
	printf("%s\n", 0x90000000);

//	int num = *(int *)0x70000000;

#if 0
	__asm__ __volatile__ (
		"mov	r0, pc\n\t"
		"mov	pc, #0x70000000\n\t"
		:::"r0"
	);
#endif
	disable_mmu();

	return 0;
}



