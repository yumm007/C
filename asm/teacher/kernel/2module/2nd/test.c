#include <linux/module.h>
#include <linux/init.h>

int test_init(void)
{
	printk("hello kernel\n");
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


