#include <linux/module.h>
#include <linux/init.h>

void foo1(void)
{
	*(int *)0 = 0;
}
void foo(void)
{
	foo1();
}
int test_init(void)
{
	printk("hahaha ....\n");
	foo();
	printk("hello kernel debug\n");

	return 0;
}
void test_exit(void)
{
	printk("bye\n");
}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");
