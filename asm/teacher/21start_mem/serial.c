#include "common.h"
#include "s3c2440.h"

#define UART_NR UART0

unsigned int __aeabi_uidiv(unsigned int num, unsigned int d)
{
	unsigned int count = 0;

	if(d == 0)
		return num;
	while(num >= d)
	{
		num -= d;
		count++;
	}

	return count;
}

static void _serial_init(int bord_rate, int data, int parity, int stop)
{
	struct gpio *const gpio = get_base_gpio();
	struct uart *const uart = get_base_uart(UART_NR);

	setval(gpio->GPHCON, 0xa, 4, 4 + 4 * UART_NR);
	setbit(gpio->GPHUP, 0x3, 2 + 2 * UART_NR);

	switch(data)
	{
		case 5:
			clrbit(uart->ULCON, 0x3, 0);
			break;
		case 6:
			setval(uart->ULCON, 0x1, 2, 0);
			break;
		case 7:
			setval(uart->ULCON, 0x2, 2, 0);
			break;
		case 8:
		default:
			setbit(uart->ULCON, 0x3, 0);
	}

	switch(parity)
	{
		case 1:
			setval(uart->ULCON, 0x4, 3, 3);
			break;
		case 2:
			setval(uart->ULCON, 0x5, 3, 3);
			break;
		case 0:
		default:
			clrbit(uart->ULCON, 0x7, 3);
	}

	switch(stop)
	{
		case 2:
			set1(uart->ULCON, 2);
			break;
		case 1:
		default:
			set0(uart->ULCON, 2);
	}

	setval(uart->UCON, 0x5, 4, 0);

	uart->UBRDIV = 50000000u / (bord_rate * 16) - 1;
}

void serial_init(void)
{
	_serial_init(115200, 8, 0, 1);
}

void serial_putc(unsigned char c)
{
	struct uart *const uart = get_base_uart(UART_NR);
	
	while(!(uart->UTRSTAT & 0x2));

	uart->UTXH = c;

	if(c == '\n')
		serial_putc('\r');
}

unsigned char serial_getc(void)
{
	struct uart *const uart = get_base_uart(UART_NR);
	
	while(!(uart->UTRSTAT & 0x1));

	unsigned char c = uart->URXH;

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


