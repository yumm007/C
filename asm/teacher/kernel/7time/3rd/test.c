#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>

struct timer_list timer;

int a;
void timer_handle(unsigned long data)
{
	printk("boom[%d]\n", *(int *)data);	
	//timer.expires = jiffies + 2*HZ;
	//add_timer(&timer);
	mod_timer(&timer, jiffies + 1*HZ);
}
int test_init(void)
{
	a = 99;
	init_timer(&timer);
	timer.expires = jiffies + 5*HZ;
	timer.function = timer_handle;
	timer.data = (unsigned long)&a;
	
	printk("hello timer\n");
	add_timer(&timer);
	return 0;
}

void test_exit(void)
{
	del_timer(&timer);
	printk("[%s]\n", __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


