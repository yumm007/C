#include <linux/module.h>
#include <linux/init.h>
#include <asm/sizes.h>
#include <asm/io.h>

unsigned long phys, virt;
unsigned long *gpecon, *gpedat, *gpeup;

void led_on(void)
{
	*gpedat &= ~(1 << 13);
}

void led_off(void)
{
	*gpedat |= (1 << 13);
}


int test_init(void)
{
	phys = 0x56000000;
	virt = ioremap(phys, SZ_4K);
	gpecon = virt + 0x40;	
	gpedat = virt + 0x44;
	gpeup = virt + 0x48;

	//configure gpe13 is output pin
	*gpecon &= ~(3 << 26);
	*gpecon |= (1 << 26);

	*gpeup |= (1 << 13);


	//on	
	led_on();
	printk("hello io\n");
	return 0;
}

void test_exit(void)
{
	led_off();
	iounmap(virt);
	printk("[%s]\n", __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


