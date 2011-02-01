#include <common.h>
#include <s3c2440.h>

#define UTRSTAT0 (*(volatile unsigned char *)0x50000010)
#define UTXH0 (*(volatile unsigned char *)0x50000020)
#define URXH0 (*(volatile unsigned char *)0x50000024)

struct com_arg {
  int com_num;
  int parity;
  int stop;
  int data_bit;
  unsigned int bund_rate;
};

void init_com(const struct com_arg *arg) {
	struct uart *const uart =	get_base_uart(arg->com_num);
	struct gpio *const gpio =  get_base_gpio();

	setval(gpio->GPHCON, 0xa, 4, 4*(arg->com_num + 1));
	setval(gpio->GPHUP, 0x3, 2, 2*(arg->com_num + 1));

	setval(uart->ULCON, arg->parity, 3, 3)
	setval(uart->ULCON, (arg->stop - 1), 1, 2)
	setval(uart->ULCON, (arg->data_bit - 5), 2, 0)

	setval(uart->UCON, 0x5, 4, 0);	/* 设置接收和发送为中断或查询模式 */
	setval(uart->UCON, 0x0, 2, 10);	/* 设置波特率采用PCLK作为时钟源 */

	uart->UBRDIV=(int)(50000000/(arg->bund_rate * 16) -1);	
}

void puts(const char *s);
int init_uart0(int uart_num)
{
	struct com_arg arg = {0, 0, 1, 8, 115200};

	init_com(&arg);
	puts("\nWelcome to y-boot.");

	return 0;
}

void serial_putc(unsigned char c)
{
	struct uart *const uart = get_base_uart(0);

	while(!(uart->UTRSTAT & 0x2));

	uart->UTXH = c;

	if(c == '\n')
		serial_putc('\r');
}

unsigned char serial_getc(void)
{
	while(!(UTRSTAT0 & 0x1));

	unsigned char c = URXH0;

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

