
typedef unsigned int u32;

void (*printf)(const char *, ...) = (void *)0x33f9291c;

#define create_descriptor(ttb, va, pa) \
		ttb[va >> 20] = (pa & 0xfff00000) | 0x12;

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
	u32 *ttb = (void *)0x31000000;

	memset((char *)ttb, 0, 4 * 4096);

	section_map(ttb, 0, 4096, 0);
	section_map(ttb, 0x30000000, 0x34000000, 0x30000000);
	section_map(ttb, 0x48000000, 0x60000000, 0x48000000);

	strcpy((void *)0x32000000, "hello mmu....");

	create_descriptor(ttb, 0x80000000, 0x32000000);
	create_descriptor(ttb, 0x90000000, 0x32000000);

	enable_mmu(ttb);

	printf("%s\n", 0x80000000);
	printf("%s\n", 0x90000000);

//	int num = *(int *)0x70000000;

	__asm__ __volatile__ (
		"mov	r0, pc\n\t"
		"mov	pc, #0x70000000\n\t"
		:::"r0"
	);

	disable_mmu();

	return 0;
}



