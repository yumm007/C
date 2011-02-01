#include <linux/module.h>
#include <linux/init.h>
#include <linux/spinlock.h>

spinlock_t key;
spinlock_t irq_key;
unsigned long flag;

void do_irq(void)
{
	spin_lock(&irq_key);			
	spin_unlock(&irq_key);
}

int test_init(void)
{
	//init
	spin_lock_init(&key);
	spin_lock_init(&irq_key);


	//互斥
	spin_lock_irqsave(&key, flag);	
	//临界区
	spin_unlock_irqrestore(&key, flag);	



	printk("[%s] mutex\n", __FUNCTION__);
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


