
typedef unsigned int u32;

static void (*printf)(const char *, ...) = (void *)0x33f9291c;

void print_stat(void)
{
	u32 fsr = 0;
	u32 far = 0;

	__asm__ __volatile__(
		"mrc p15, 0, %[fsr], c5, c0, 0\n"
		"mrc p15, 0, %[far], c6, c0, 0\n"	
		: [fsr] "+r" (fsr), [far] "+r" (far)
	);					

	printf("faddr = %08x\n", far);

	printf("fstat = %08x\nFault type: ", fsr);	

	int ft = fsr & 0xf;	
	switch( ft ) {
		case 0x1:
		case 0x3:
			printf("align.\n");
			break;
		case 0x5:
		case 0x7:
			printf("Translation.\n");
			break;
		case 0x9:
		case 0xb:
			printf("domain.\n");
			break;
		case 0xd:
		case 0xf:
			printf("permission.\n");
			break;
		default:
			printf("error.\n");
	} 
}
