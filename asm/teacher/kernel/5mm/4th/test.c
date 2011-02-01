#include <linux/module.h>
#include <linux/init.h>
#include <asm/sizes.h>
#include <asm/io.h>


unsigned long phys, virt;
unsigned long *gpecon, *gpedat, *gpeup;
unsigned int reg;


void led_on(void)
{
	iowrite32(ioread32(gpedat) & ~(3 << 12), gpedat);
}

void led_off(void)
{
	iowrite32(ioread32(gpedat) | (3 << 12), gpedat);
}


int test_init(void)
{
	virt = 0xeeee0000;
	gpecon = virt + 0x40;	
	gpedat = virt + 0x44;
	gpeup = virt + 0x48;

	//configure gpe13 is output pin
	reg = ioread32(gpecon);	
	reg &= ~(0xf << 24);
	reg |= (5 << 24);
	iowrite32(reg, gpecon);

	iowrite32(ioread32(gpeup) | (3 << 12), gpeup);

	//on	
	led_on();
	printk("hello io\n");
	return 0;
}

void test_exit(void)
{
	led_off();
	printk("[%s]\n", __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


