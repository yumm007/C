#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/interrupt.h>

struct tasklet_struct task;

/* 中断处理函数 */
irqreturn_t key_irq_handler(int irq, void *dev_id)
{
	printk("key down\n");
	tasklet_schedule(&task);
	return IRQ_HANDLED;	
}

void task_func(unsigned long data)
{
	printk("%s\n", __FUNCTION__);	
}

int test_init(void)
{
	int ret;
	
	//init
	tasklet_init(&task, task_func, NULL);	
	
	//注册
	ret = request_irq(IRQ_EINT1, key_irq_handler, IRQF_TRIGGER_FALLING, "1st:test irq", NULL);	
	if(ret){
		printk("request_irq failed\n");
		ret = -EBUSY;
		goto err;
	}
	printk("hello irq\n");
	return 0;

err:
	return ret;
}

void test_exit(void)
{
	free_irq(IRQ_EINT1, NULL);
	printk("[%s]\n", __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


