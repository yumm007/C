#include <common.h>

#define GPHCON	(*(volatile unsigned int *)0x56000070)
#define GPHUP	(*(volatile unsigned int *)0x56000078)


#define UART_NR 0

#define UTXH 	(*(volatile unsigned char *)(0x50000020 + 0x4000 * UART_NR))
#define URXH 	(*(volatile unsigned char *)(0x50000024 + 0x4000 * UART_NR))
#define UTRSTAT (*(volatile unsigned int *)(0x50000010 + 0x4000 * UART_NR))

#define	ULCON 	(*(volatile unsigned int *)(0x50000000 + 0x4000 * UART_NR))
#define	UCON	(*(volatile unsigned int *)(0x50000004 + 0x4000 * UART_NR))
#define UBRDIV	(*(volatile unsigned int *)(0x50000028 + 0x4000 * UART_NR))

extern unsigned int get_pclk(void);

static void _serial_init(int bord_rate, int data, int parity, int stop)
{
	setval(GPHCON, 0xa, 4, 4 + 4 * UART_NR);
	setbit(GPHUP, 0x3, 2 + 2 * UART_NR);

	switch(data)
	{
		case 5:
			clrbit(ULCON, 0x3, 0);
			break;
		case 6:
			setval(ULCON, 0x1, 2, 0);
			break;
		case 7:
			setval(ULCON, 0x2, 2, 0);
			break;
		case 8:
		default:
			setbit(ULCON, 0x3, 0);
	}

	switch(parity)
	{
		case 1:
			setval(ULCON, 0x4, 3, 3);
			break;
		case 2:
			setval(ULCON, 0x5, 3, 3);
			break;
		case 0:
		default:
			clrbit(ULCON, 0x7, 3);
	}

	switch(stop)
	{
		case 2:
			set1(ULCON, 2);
			break;
		case 1:
		default:
			set0(ULCON, 2);
	}

	setval(UCON, 0x5, 4, 0);

	UBRDIV = get_pclk() / (bord_rate * 16) - 1;
}

void serial_init(void)
{
	_serial_init(115200, 8, 0, 1);
}


#define UTXH0 	(*(volatile unsigned char *)0x50000020)
#define UTRSTAT0 (*(volatile unsigned int *)0x50000010)

void serial_putc(unsigned char c)
{
	while(!(UTRSTAT & 0x2));

	UTXH = c;

	if(c == '\n')
		serial_putc('\r');
}

unsigned char serial_getc(void)
{
	while(!(UTRSTAT & 0x1));

	unsigned char c = URXH;

	if(c == '\r')
		c = '\n';

	serial_putc(c);

	return c;
}

void puts(const char *s)
{
	while(*s)
		serial_putc(*s++);

	serial_putc('\n');
}

void gets(char *s)
{
	char c;
	while((c = serial_getc()) != '\n')
		*s++ = c;
	*s = 0;
}
