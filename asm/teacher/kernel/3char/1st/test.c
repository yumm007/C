#include <linux/module.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/cdev.h>

unsigned int major = 0;
unsigned int minor = 0;
static dev_t devno;
static struct cdev t_cdev;

static struct file_operations test_fpos;
int test_init(void)
{
	//old
	//register_chrdev(253, "test char driver", &test_fpos);
	
	//new
	if(major)
	{
		devno = MKDEV(major, minor);	
		register_chrdev_region(devno, 1, "new char");
	}else{
		alloc_chrdev_region(&devno, minor, 1, "new alloc char");			
		printk("major:[%d], minor[%d]\n", MAJOR(devno), MINOR(devno));
	}
	cdev_init(&t_cdev, &test_fpos);
	cdev_add(&t_cdev, devno, 1);
	return 0;
}

void test_exit(void)
{
	//new
	cdev_del(&t_cdev);	
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


