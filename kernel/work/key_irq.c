#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

#define 	IRQ_NUM	IRQ_EINT1

struct key_irq_t {
	unsigned long args;	
	struct work_struct ws;
}kt = {
	.args = 0,
};

static void work_fun(struct work_struct *work) {
	printk("%lu: received sig.\n", container_of(work, struct key_irq_t, ws)->args);
}

static irqreturn_t key_handler(int unused, void *p) {

	schedule_work(&kt.ws);
	return IRQ_HANDLED;
}

int key_irq_init(void) {
	int ret;

	INIT_WORK(&kt.ws, work_fun);

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
	printk(KERN_INFO "module exit.\n");
}

module_init(key_irq_init);
module_exit(key_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
