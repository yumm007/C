
typedef unsigned int u32;

void (*printf)(const char *, ...) = (void *)0x33f9291c;

#define create_descriptor(ttb, va, pa) \
		ttb[va >> 20] = (pa & 0xfff00000) | 0x12;

void sleep(unsigned int loops)
{
	loops *= 1000000;

	__asm__ __volatile__ (
		"1:\n\t"
		"subs %0, %1, #1\n\t"
		"bne 1b"
		:"=r"(loops)
		:"0"(loops)
	);
}

void section_map(u32 *ttb, u32 va_start, u32 va_end, u32 pa)
{
	while(va_start < va_end)
	{
		create_descriptor(ttb, va_start, pa);
		
		va_start += 0x100000;
		pa += 0x100000;
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

int main(void)
{
	u32 *ttb1 = (void *)0x31000000;
	u32 *ttb2 = (void *)0x31010000;

	memset((char *)ttb1, 0, 4 * 4096);
	memset((char *)ttb2, 0, 4 * 4096);

	section_map(ttb1, 0, 4096, 0);
	section_map(ttb1, 0x30000000, 0x34000000, 0x30000000);
	section_map(ttb1, 0x48000000, 0x60000000, 0x48000000);

	section_map(ttb2, 0, 4096, 0);
	section_map(ttb2, 0x30000000, 0x34000000, 0x30000000);
	section_map(ttb2, 0x48000000, 0x60000000, 0x48000000);

	strcpy((void *)0x32000000, "111111111111111111");
	strcpy((void *)0x33000000, "222222222222222222");

	create_descriptor(ttb1, 0x80000000, 0x32000000);

	create_descriptor(ttb2, 0x80000000, 0x33000000);

	enable_mmu(ttb1);

	u32 *tmp = ttb1;
	u32 flag = 0;

	while(1)
	{
		printf("%s\n", 0x80000000);
			
		if(flag == 1)
			tmp = ttb1;
		else
			tmp = ttb2;
		
		flag = !flag;

		__asm__ __volatile__ (
			"mcr	p15, 0, %0, c2, c0, 0\n\t"
			"mov	r0, #0\n\t"
			"mcr	p15, 0, r0, c8, c7, 0\n\t"
			::"r"(tmp)
			:"r0"
		);
		
		sleep(2);
	}

	disable_mmu();

	return 0;
}



