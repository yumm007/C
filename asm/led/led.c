#define GPECON 0x56000040
#define GPEDAT 0x56000044

void (*printf)(const char *fmt, ...) = (void *) 0x33f9291c;

volatile unsigned int *gpecon = (void *) GPECON;
volatile unsigned int *gpedat = (void *) GPEDAT;

static int delay(int sec);
static void shine0(void);
static void shine1(void);

int main(void)
{
	
	while (1) {
		shine0();
		delay(2);
		shine1();
		delay(2);
	}
	return 0;
}

static int delay(int sec) {
	unsigned int tim = sec * 1000000;
	while (tim)
		tim--;	
	return tim;
}

static void shine0(void) {
	*gpecon = ((*gpecon) & 0xf0ffffff) | (0x1 << 24);
	*gpedat = ((*gpedat) & 0xcfff) | (1 << 13);
}

static void shine1(void) {
	*gpecon = ((*gpecon) & 0xf0ffffff) | (0x1 << 26);
	*gpedat = ((*gpedat) & 0xcfff) | (1 << 12);
}
