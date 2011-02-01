#include <linux/module.h>
#include <linux/init.h>
#include <asm/io.h>
#include <asm/sizes.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/delay.h>

#define	GPEBASE	0xeeee0000
#define	GPECON	(0x40/4)
#define	GPEDAT	(0x44/4)
#define	GPEUP 	(0x48/4)
#define	LED_ON	'1'
#define	LED_OFF	'0'
#define	DEV_MAJOR 253
#define	DEV_MINOR 0

struct led_dev_t {
	unsigned int gpe_led_con_old;
	unsigned int gpe_led_dat_old;
	unsigned int gpe_led_up_old;
	volatile unsigned int *gpebase;

	dev_t dev_num;
	struct file_operations fops;
	struct cdev cdev_info;
};
static struct led_dev_t led;

static void init_led_dev(void) {
	//led.gpebase = (volatile unsigned int *)(ioremap(GPEBASE, SZ_4K));
	led.gpebase = (volatile unsigned int *)(GPEBASE);
	led.gpe_led_con_old = (*(led.gpebase + GPECON) & (3 << 26));
	led.gpe_led_dat_old = (*(led.gpebase + GPEDAT) & (1 << 13));
	led.gpe_led_up_old = (*(led.gpebase + GPEUP) & (1 << 13));

	*(led.gpebase + GPECON) &= ~(3 << 26);
	*(led.gpebase + GPECON) |= (1 << 26);
	*(led.gpebase + GPEUP) |= (1 << 13); 
}

static void led_on(void) {
	*(led.gpebase + GPEDAT) &= ~(1 << 13); 
}
static void led_off(void) {
	*(led.gpebase + GPEDAT) |= (1 << 13); 
}

static ssize_t led_read(struct file *files, char __user *user_buf, size_t read_count, loff_t *offt) {
	if (*(led.gpebase + GPEDAT) & (1 << 13))
		return LED_OFF;
	return LED_ON;
}
static ssize_t led_write(struct file *files, const char __user *user_buf, size_t write_count, loff_t *offt) {
	size_t i = 0, ret;
	char *kbuf;
	if ((kbuf = kmalloc(write_count, GFP_KERNEL)) == NULL)
		return -ENOBUFS;
	ret = copy_from_user(kbuf, user_buf, write_count);
	for (i = 0; i < write_count; i++) {
		switch (kbuf[i]) {
			case LED_ON:
				led_on();
				mdelay(1000);
				break;
			case LED_OFF:
				led_off();
				mdelay(1000);
				break;
			default:
				break;
		}
	}
	kfree(kbuf);
	return ret;
}
static int led_open(struct inode *node, struct file *files) {
	return 0;
}
static int led_close(struct inode *node, struct file *files) {
	return 0;
}

static void restore_led_dev(void) {
	*(led.gpebase + GPECON) |= led.gpe_led_con_old;
	*(led.gpebase + GPEDAT) |= led.gpe_led_dat_old;
	*(led.gpebase + GPEUP)  |= led.gpe_led_up_old;
}

static int reg_dev(void) {
   led.dev_num = MKDEV(DEV_MAJOR, DEV_MINOR);
   if (register_chrdev_region(led.dev_num, 1, "LED device") != 0) {
      printk(KERN_WARNING "register char device failed. usr alloc to register it.\n");
      if (alloc_chrdev_region(&led.dev_num, 0 ,1 , "This is test char device") != 0) {
         printk(KERN_WARNING "register char device by alloc failed, module exit.\n");
         return -1; 
      } else
         printk(KERN_WARNING "the major number is %d, minor number is %d.\n", MAJOR(led.dev_num), MINOR(led.dev_num));
   }   

   cdev_init(&led.cdev_info, &led.fops);
	cdev_add(&led.cdev_info, led.dev_num, 1);
	/*if error*/
	return 0;
}

static int __init led_init(void) {
	
	if (reg_dev() != 0)
		return -1;

	led.fops.open 		= led_open;
	led.fops.release 	= led_close;
	led.fops.read 		= led_read;
	led.fops.write 	= led_write;
	
	init_led_dev();
	return 0;
}

static void __exit led_exit(void) {
	restore_led_dev();
	unregister_chrdev_region(led.dev_num, 1);
	cdev_del(&led.cdev_info);
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
