#include <s3c2440.h>

void gpio_init(void)
{
	/* set up the I/O ports */
	struct gpio * const gpio = get_base_gpio();

   gpio->GPACON = 0x007FFFFF;
   gpio->GPBCON = 0x00044555;
   gpio->GPBUP = 0x000007FF;
   gpio->GPCCON = 0xAAAAAAAA;
   gpio->GPCUP = 0x0000FFFF;
   gpio->GPDCON = 0xAAAAAAAA;
   gpio->GPDUP = 0x0000FFFF;
   gpio->GPECON = 0xAAAAAAAA;
   gpio->GPEUP = 0x0000FFFF;
   gpio->GPFCON = 0x000055AA;
   gpio->GPFUP = 0x000000FF;
   gpio->GPGCON = 0xFF95FFBA;
   gpio->GPGUP = 0x0000FFFF;
   gpio->GPHCON = 0x002AFAAA;
   gpio->GPHUP = 0x000007FF;

}
