#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/timer.h>

#define IRQ_NUM	IRQ_EINT1
#define	SEC	2

struct key_irq_t {
	atomic_t led_status;
	atomic_t count;
	struct timer_list tl;
}kt = {
	.led_status = ATOMIC_INIT(0),
	.count = ATOMIC_INIT(0),
};

static void timer_fun(unsigned long args);
static void pub_add_timer(void) {
	atomic_inc(&kt.count);
	kt.tl.expires = jiffies + SEC * HZ;
	kt.tl.function = timer_fun;
	kt.tl.data	= (unsigned long)&kt.count;
	add_timer(&kt.tl);
}

static void timer_fun(unsigned long args) {
	printk("%d: received sig.\n", *(int *)args);
	pub_add_timer();
}

static irqreturn_t key_handler(int unused, void *p) {
	pub_add_timer();
	return IRQ_HANDLED;
}

int key_irq_init(void) {
	int ret;

	init_timer(&kt.tl);

	ret = request_irq(IRQ_NUM, key_handler, IRQF_TRIGGER_RISING, "irq for key", NULL);
	if (ret == -EBUSY) {
		printk(KERN_WARNING "EBUSY: request irq failed.\n");
		return -EBUSY;
	} else if (ret < 0) {
		printk(KERN_WARNING "Unknow: request irq failed.\n");
		return ret;
	}

	printk(KERN_INFO "init successed.\n");
	return 0;
}

void key_irq_exit(void) {
	free_irq(IRQ_NUM, NULL);
	del_timer(&kt.tl);
	printk(KERN_INFO "module exit.\n");
}

module_init(key_irq_init);
module_exit(key_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
