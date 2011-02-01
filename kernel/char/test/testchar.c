#include <linux/module.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/signal.h>

#define KBUFSIZE	256
#define DEV_MAJOR		204
#define DEV_MINOR		64

typedef struct{
	dev_t	dev_num;						/* 设备号  */
	struct cdev cdev_info;			/* 设备信息，包括owner，引用数等等*/
	struct file_operations fops;	/*	操作方法集合*/
	struct file *filep;
	wait_queue_head_t *q;
	
	
	char buf[KBUFSIZE];				/*	BUF*/
	int size;							/* 当前大小*/
	
}char_dev_t;
static char_dev_t ct;

static ssize_t char_dev_read(struct file *filep, char __user *user_buf, size_t read_count, loff_t *offt) {
	char_dev_t *dev = filep->private_data;
	if(wait_event_interruptible(dev->q, ((dev->size - (*offt) ) >= read_count)))		
		return -ERESTARTSYS; //wait返回非0时，表示被信号惊醒，直接返回-ERESTARTSYS;
	
	copy_to_user(buf, dev->buf[*offt], read_count);
	*offt += read_count;

	return 0;
}

static ssize_t char_dev_write(struct file *filep, const char __user *user_buf, size_t write_count, loff_t *offt) {

	
	return 0;
}
static int char_dev_open(struct inode *inode, struct file *filep) {
	//init hardware
	char_dev_t *tmp = (char_dev_t *)container_of(inode->i_cdev, char_dev_t, cdev_info);
	filep->private_data = (void *)tmp;

	printk(KERN_WARNING "init char device successful.\n");
	return 0;
}
static int char_dev_close(struct inode *inode, struct file *filep) {
	//close hardware
	printk(KERN_WARNING "close device successful.\n");
	return 0;
}

int test_init(void) {
	ct.dev_num = MKDEV(DEV_MAJOR, DEV_MINOR);
	if (register_chrdev_region(ct.dev_num, 1, "This is test char device") != 0) {
		printk(KERN_WARNING "register char device failed. usr alloc to register it.\n");
		if (alloc_chrdev_region(&ct.dev_num, 0 ,1 , "This is test char device") != 0) {
			printk(KERN_WARNING "register char device by alloc failed, module exit.\n");
			return -1;
		} else
			printk(KERN_WARNING "the major number is %d, minor number is %d.\n", MAJOR(ct.dev_num), MINOR(ct.dev_num));
	}

	cdev_init(&ct.cdev_info, &ct.fops);
	ct.fops.read = char_dev_read;
	ct.fops.write = char_dev_write;
	ct.fops.open = char_dev_open;
	ct.fops.release = char_dev_close;

	ct.size = 0;

	cdev_add(&ct.cdev_info, ct.dev_num, 1);
	printk(KERN_WARNING "testchar init.\n");
	return 0;
}

void test_exit(void) {
	printk(KERN_WARNING "testchar exit.\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v1.2");
