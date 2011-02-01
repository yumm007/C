#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/interrupt.h>

/* 中断处理函数 */
irqreturn_t key_irq_handler(int irq, void *dev_id)
{
	printk("key down [%d]\n", *(int *)dev_id);
	return IRQ_HANDLED;	
}

irqreturn_t key_irq_handler1(int irq, void *dev_id)
{
	printk("key down [%d]\n", *(int *)dev_id);
	return IRQ_HANDLED;	
}

static int a, b;
int test_init(void)
{
	int ret;
	a = 88;
	b = 99;
	//注册
	ret = request_irq(IRQ_EINT1, key_irq_handler, IRQF_TRIGGER_FALLING | IRQF_SHARED, "1st:test irq", &a);	
	if(ret){
		printk("request_irq 0 failed\n");
		ret = -EBUSY;
		goto err;
	}

	ret = request_irq(IRQ_EINT1, key_irq_handler1, IRQF_TRIGGER_FALLING | IRQF_SHARED, "2nd:test irq", &b);	
	if(ret){
		printk("request_irq 1 failed\n");
		ret = -EBUSY;
		goto err1;
	}
	printk("hello irq\n");
	return 0;
err1:
	free_irq(IRQ_EINT1, &a);
err:
	return ret;
}

void test_exit(void)
{
	free_irq(IRQ_EINT1, &a);
	free_irq(IRQ_EINT1, &b);
	printk("[%s]\n", __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


