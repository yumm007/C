#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/semaphore.h>
#include <linux/device.h>

#include <asm/io.h>
#include <asm/sizes.h>

#include "led_ioctl.h"

struct _led_t{
	unsigned long virt, phys;	
	unsigned long gpecon, gpedat, gpeup;

	void (*on)(struct _led_t *);
	void (*off)(struct _led_t *);
	void (*config)(struct _led_t *);

	struct cdev l_cdev;
	dev_t devno;

	struct semaphore sem;
};

/*****************
 * ARM
 *****************/
void s3c_led_config(struct _led_t *l)
{
	unsigned int reg;		
	reg = ioread32(l->gpecon);
	reg &= ~(3 << 26);
	reg |= (1 << 26);
	iowrite32(reg, l->gpecon);

	iowrite32(ioread32(l->gpeup) | (1 << 13), l->gpeup);
}


void s3c_led_on(struct _led_t *l)
{
	iowrite32(ioread32(l->gpedat) & ~(1 << 13), l->gpedat);
}

void s3c_led_off(struct _led_t *l)
{
	iowrite32(ioread32(l->gpedat) | (1 << 13), l->gpedat);
}

/***********************
 * init led object
 ************************/
static int init_led_device(struct _led_t *l)
{
	l->phys = 0x56000000;	
	l->virt = ioremap(l->phys, SZ_4K);

	l->gpecon = l->virt + 0x40;
	l->gpedat = l->virt + 0x44;
	l->gpeup = l->virt + 0x48;

	l->on = s3c_led_on;
	l->off = s3c_led_off;
	l->config = s3c_led_config;

	sema_init(&l->sem, 1);
	return 0;	
}

static void destroy_led_device(struct _led_t *l)
{
	iounmap(l->virt);	
}

/*************
 * Char
 *************/
int led_ioctl (struct inode *node, struct file *filp, unsigned int cmd, unsigned long args)
{
	struct _led_t *l = container_of(node->i_cdev, struct _led_t, l_cdev);
	if(down_interruptible(&l->sem))	
		return -ERESTARTSYS;

	switch(cmd)
	{
		case LED_ON:
			l->on(l);
			break;

		case LED_OFF:
			l->off(l);
			break;

		case LED_CONFIG:
			l->config(l);
			break;

		default:
			printk("unknow ioctl cmd\n");
			up(&l->sem);
			return -EINVAL;

	}

	up(&l->sem);
	return 0;	
}

static struct file_operations fops = {
	.ioctl = led_ioctl,
};

/********
 * Class
 *********/
struct class *my_class;
static int create_my_class(void)
{
	my_class =class_create(THIS_MODULE, "my_class");	
	device_create(my_class, NULL, MKDEV(253, 0), NULL, "%s", "test-led");
}

static void destroy_my_class(void)
{
	device_destroy(my_class, MKDEV(253, 0));
	class_destroy(my_class);
}

/******************
 * Module
 ******************/
static struct _led_t *led;
static int led_driver_init(void)
{
	int ret;
	led = kmalloc(sizeof(struct _led_t), GFP_KERNEL);
	if(NULL == led)
	{
		printk("<kernel> kmalloc failed\n");
		ret = - ENOMEM;
		goto err;
	}

	init_led_device(led);	

	//char
	led->devno = MKDEV(253, 0);
	register_chrdev_region(led->devno, 1, "my led driver");			

	cdev_init(&led->l_cdev, &fops);

	ret = cdev_add(&led->l_cdev, led->devno, 1);
	if(ret)
	{
		printk("<kernel> cdev add failed\n");
		ret = - EBUSY;
		goto err1;
	}

	create_my_class();

	printk("%s\n", __FUNCTION__);
	return 0;
err1:
	unregister_chrdev_region(led->devno, 1);	
	destroy_led_device(led);
	kfree(led);
err:
	return ret;
}

static void led_driver_exit(void)
{
	destroy_my_class();

	cdev_del(&led->l_cdev);
	unregister_chrdev_region(led->devno, 1);	
	destroy_led_device(led);
	kfree(led);
	printk("%s\n", __FUNCTION__);
}

module_init(led_driver_init);
module_exit(led_driver_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richar");
MODULE_VERSION("v0.1");
