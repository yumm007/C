#include <linux/module.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/signal.h>

#define MAJOR_NUMBER		253	
#define MANOR_NUMBER		0
#define DEVICE_COUNT		1
#define KBUF_SIZE			256

#define LEN_ON		0
#define LEN_OFF	1

typedef struct char_dev {
	dev_t		devno;
	struct	cdev c_cdev;
	struct	file_operations fops;

	char		buf[KBUF_SIZE];
	int		offset;
};

static char_dev cdev_str;

int testchar_open(struct inode * node, struct file * files) {
	return 0;
}

ssize_t testchar_read(struct file * files, char __user *buf, size_t n, loff_t *offt) {
	return n - copy_to_user(buf, "data from kernel", 20);
}

ssize_t testchar_write(struct file * files, const char __user * buf, size_t n, loff_t *offt) {
	char kbuf[KBUF_SIZE];
	size_t len = n;

	if (len > KBUF_SIZE)
		len = KBUF_SIZE;
	len = copy_from_user(kbuf, buf, len);

	printk(KERN_WARNING"in driver_write [%s]\n", kbuf);
	return n - len;
}

static int led_on(void) {
	printk(KERN_WARNING"led on.\n");
	return 0;
}

static int led_off(void) {
	printk(KERN_WARNING"led off.\n");
	return 0;
}

int testchar_ioctl(struct inode *node, struct file * files, unsigned int cmd, unsigned long args) {
	int ret;

	switch (cmd) {
		case LEN_ON:
			ret = led_on();
			break;
		case LEN_OFF:
			ret = led_off();
			break;
		default:
			printk(KERN_WARNING"unknow ioctl cmd\n");
			ret = -EINVAL;
			break;
	}

	return ret;
}

int testchar_init(void) {
	devno = MKDEV(MAJOR_NUMBER, MANOR_NUMBER);
	if ( register_chrdev_region(devno, DEVICE_COUNT, "Test char device.") != 0) 
		goto err_reg;

	fops.open = testchar_open;
	fops.read = testchar_read;
	fops.write = testchar_write;
	fops.ioctl = testchar_ioctl;

	cdev_init(&c_cdev, &fops);
	if (cdev_add(&c_cdev, devno, DEVICE_COUNT) != 0)
		goto err_add;
	
	return 0;

err_add:
	unregister_chrdev_region(devno, DEVICE_COUNT); 
err_reg:
	return -EBUSY;
}

void testchar_exit(void) {
	cdev_del(&c_cdev);
	unregister_chrdev_region(devno, DEVICE_COUNT);
}

module_init(testchar_init);
module_exit(testchar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
