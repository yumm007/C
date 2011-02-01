#include <linux/module.h>
#include <linux/init.h>
#include <linux/semaphore.h>

int test_init(void)
{
	int ret = -1;
	struct semaphore sem;
	sema_init(&sem, 1);	

	if(down_interruptible(&sem))
		return -ERESTARTSYS;
	if(ret)
	{
		
		up(&sem);
		return -1;
	}
	//...
	up(&sem);

	printk("[%s] semaphore\n", __FUNCTION__);
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


