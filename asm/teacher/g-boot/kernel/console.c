#include <common.h>
#include <ctype.h>

extern void serial_init(void);
extern int serial_getc(void);
extern int serial_putc(const char);

struct console_buffer{
	int size;
	int head;
	int tail;
	char buf[CONFIG_SYS_CBSIZE];
}queue;

static void console_puts(const char *s)
{
	while(*s)
		serial_putc(*s++);
}

static int readline_into_buffer(void)
{
	static char buf[CONFIG_SYS_CBSIZE];
	int n = 0;
	char c;
	int i;

	while(1)
	{
		c = serial_getc();

		switch(c)
		{
			case '\r':		/* Enter */
			case '\n':
				buf[n] = '\n';
				serial_putc('\n');
				n++;
				goto done;

			case '\0':		/* nul	*/
				continue;

			case 0x03:		/* ^C - break	*/
				serial_putc('\n');
				return -1;

			case 0xc:		/* ^l clear screen */
				serial_putc(0xc);
				buf[n] = '\0';
				console_puts(CONFIG_SYS_PROMPT" ");
				console_puts(buf);
				continue;

			case 0x15:		/* ^U - erase line */
				while(n > 0)
				{
					console_puts("\b \b");
					n--;
				}
				continue;

			case 0x08:		/* ^H  - backspace	*/
			case 0x7F:		/* DEL - backspace	*/
				if(n > 0)
				{
					console_puts("\b \b");
					n--;
				}
				continue;

			default:
				if(isprint(c) && (n < CONFIG_SYS_CBSIZE - 1))
				{
					serial_putc(c);
					buf[n] = c;
					n++;
				}
		}
	}

done:
	for(i = 0; i < n && (queue.tail + 1) != queue.head; i++){
		queue.buf[queue.tail++] = buf[i];
		if(queue.tail >= queue.size)
			queue.tail = 0;
	}

	return 0;
}

void console_init(void)
{
	queue.size = CONFIG_SYS_CBSIZE;
	queue.head = 0;
	queue.tail = 0;
	
	serial_init();
}

int console_getc(void)
{
	if(queue.head == queue.tail)
		if(readline_into_buffer() == -1)
			return -1;
	int i = queue.head;
	if(i + 1 == queue.size)
		queue.head = 0;
	else
		queue.head++;

	return queue.buf[i];
}

int console_putc(const char c)
{
	return serial_putc(c);
}

