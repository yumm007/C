#define GPECON 0x56000040
#define GPEDAT 0x56000044

void (*printf)(const char *fmt, ...) = (void *) 0x33f9291c;

volatile unsigned int *gpecon = (void *) GPECON;
volatile unsigned int *gpedat = (void *) GPEDAT;
volatile unsigned int *keycon = (void *) 0x56000050;
volatile unsigned int *keydat = (void *) 0x56000054;

static int delay(int sec);
static void shine0(void);
static void shine1(void);
static void check_key(void);

static unsigned int sec = 10;

int main(void)
{
	
	while (1) {
		shine0();
		delay(sec);
		shine1();
		delay(sec);
	}
	return 0;
}

static int delay(int sec) {
	unsigned int tim = sec * 10000;
	while (tim) {
		tim--;
		check_key();
	}
	return tim;
}

static void check_key(void) {
	static unsigned int old0 = 1, old1 = 2; 

	*keycon &= 0xfff0;
	
	if (((*keydat) & 0x1) != old0) {
		old0 = (*keydat) & 0x1;
		sec++;
	} 
	if (((*keydat) & 0x2) != old1) {
		old1 = (*keydat) & 0x2;
		sec--;
		if (sec == 0)
			sec = 1;
	} 

}

static void shine0(void) {
	*gpecon = ((*gpecon) & 0xf0ffffff) | (0x1 << 24);
	*gpedat = ((*gpedat) & 0xcfff) | (1 << 13);
}

static void shine1(void) {
	*gpecon = ((*gpecon) & 0xf0ffffff) | (0x1 << 26);
	*gpedat = ((*gpedat) & 0xcfff) | (1 << 12);
}
