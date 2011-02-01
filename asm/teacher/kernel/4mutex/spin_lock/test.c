#include <linux/module.h>
#include <linux/init.h>
#include <linux/spinlock.h>

int test_init(void)
{
	spinlock_t key;

	//init
	spin_lock_init(&key);


	//互斥
	spin_lock(&key);	
	//临界区
	spin_unlock(&key);	

	printk("[%s] spin lock\n", __FUNCTION__);
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


