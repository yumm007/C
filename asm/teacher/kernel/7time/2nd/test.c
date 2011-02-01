#include <linux/module.h>
#include <linux/init.h>

#include <linux/delay.h>

int test_init(void)
{
	printk("before time[%d]\n", jiffies);
	mdelay(1000);
	printk("after time[%d]\n", jiffies);

	printk("before time[%d]\n", jiffies);
	ssleep(1);
	printk("after time[%d]\n", jiffies);

	printk("hello time\n");
	return 0;
}

void test_exit(void)
{
	printk("[%s]\n", __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


