#include "s3c2440.h"

void (*printf)(const char *, ...) = (void *)0x33f9291c;

void msleep(unsigned int loops);

void _start(void)
{
	struct uart *const uart = get_base_uart(0);
	struct dmas *const dmas = get_base_dmas();

	char buf[] = "123456789\r\n";
	
	printf("dma start.....\n");

	uart->UCON = 9;
	
	dmas->dma[0].DISRC = (int)buf;
	dmas->dma[0].DISRCC = 0;

	dmas->dma[0].DIDST = (int)&uart->UTXH;
	dmas->dma[0].DIDSTC = 3;

	dmas->dma[0].DCON = 0x01c00000 | sizeof(buf);

	dmas->dma[0].DMASKTRIG = 2; 

	msleep(10);

	while(dmas->dma[0].DSTAT & 0xfffff);

	uart->UCON = 5;

	printf("dma end.....\n");
}

void msleep(unsigned int loops)
{
	loops *= 100000;

	__asm__ __volatile__ (
		"1:\n\t"
		"subs %0, %1, #1\n\t"
		"bne 1b"
		:"=r"(loops)
		:"0"(loops)
	);
}


