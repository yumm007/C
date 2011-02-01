#include <linux/module.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/errno.h>

#include <asm/uaccess.h>


#define DEV_SIZE	50
struct test_dev_t{
	//vloatie unsigned long reg;	
	//unsigned int irq;
	char kbuf[DEV_SIZE];

	struct cdev t_cdev;
};

unsigned int major = 253;
unsigned int minor = 0;
static dev_t devno;



void test_device_init(void)
{
	//dummy: init device reg *0x56000000 = 0xfffff;
	printk("<arm>init device\n");
}

int test_open (struct inode *node, struct file *filp)
{
	struct test_dev_t *dev = container_of(node->i_cdev, struct test_dev_t, t_cdev);
	filp->private_data = (void *)dev;		
	test_device_init();
	return 0;	
}

ssize_t test_read (struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	int remain;
	int ret;
	struct test_dev_t *dev = (struct test_dev_t *)filp->private_data;
	remain = copy_to_user(buf, dev->kbuf + filp->f_pos, count);		
	ret = count - remain;
	*offset += ret;
	return ret;	
}
ssize_t test_write (struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	int remain;
	int ret;
	struct test_dev_t *dev = (struct test_dev_t *)filp->private_data;
	remain = copy_from_user(dev->kbuf + filp->f_pos, buf, count);		
	//dummy: set hardware
	printk("<kernel>[%s]\n", dev->kbuf + filp->f_pos);
	ret = count - remain;
	*offset += ret;
	return ret;	
}

loff_t test_llseek(struct file *filp, loff_t offset, int whence)
{
	loff_t old = filp->f_pos;
	
	switch(whence){
		case SEEK_SET:
			filp->f_pos = offset;
			break;
		case SEEK_CUR:
			filp->f_pos += offset;
			break;
		case SEEK_END:
			filp->f_pos = DEV_SIZE + offset;
			break;
		default:
			printk("unknow whence of llseek \n");
	}		
	if((filp->f_pos < 0) || (filp->f_pos > DEV_SIZE))	
		filp->f_pos = old;
	
	return filp->f_pos;
}

static struct file_operations test_fpos = {
	.open = test_open,	
	.read = test_read,
	.write = test_write,
	.llseek = test_llseek,
};

struct test_dev_t test;

int test_init(void)
{
	int ret;
	//old
	//register_chrdev(253, "test char driver", &test_fpos);
	
	//new
	if(major)
	{
		devno = MKDEV(major, minor);	
		ret = register_chrdev_region(devno, 1, "new char");

	}else{
		ret = alloc_chrdev_region(&devno, minor, 1, "new alloc char");			
		printk("major:[%d], minor[%d]\n", MAJOR(devno), MINOR(devno));
	}
	if(ret)
	{
		ret = -EBUSY;
		printk("register region failed\n");
		goto err;
	}
	cdev_init(&test.t_cdev, &test_fpos);
	ret = cdev_add(&test.t_cdev, devno, 1);
	if(ret)
	{
		ret = -EBUSY;
		goto err1;
	}
	
	return 0;

err1:
	unregister_chrdev_region(devno, 1);	

err:
	printk("test_init failed\n");
	return ret;
}

void test_exit(void)
{
	//new
	cdev_del(&test.t_cdev);	
	unregister_chrdev_region(devno, 1);	

	//old
	//unregister_chrdev(major, "test char driver");	
	printk("[%s]\n", __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


