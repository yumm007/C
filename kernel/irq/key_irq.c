#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#define IRQ_NUM	IRQ_EINT1

static irqreturn_t key_handler(int unused, void *p) {
	printk("received sig.\n");
	return IRQ_HANDLED;
}

int key_irq_init(void) {
	int ret;

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
