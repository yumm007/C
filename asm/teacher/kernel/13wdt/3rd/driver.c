#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/semaphore.h>

#include <asm/io.h>
#include <asm/sizes.h>

#include "wdt.h"

struct _wdt_t {
	unsigned long phys, virt;
	unsigned long wtcon, wtdat, wtcnt;
	int flag;
	struct semaphore sem;

	void (*init_reset)(struct _wdt_t *);
	void (*init_timer)(struct _wdt_t *);
	void (*timer_start)(struct _wdt_t *);
	void (*timer_stop)(struct _wdt_t *);
	void (*mode)(struct _wdt_t *);
	void (*feed)(struct _wdt_t *);
};

/*******
 * Arm
 ********/
void s3c_wdt_feed(struct _wdt_t *w)
{
	iowrite32(0x8000, w->wtcnt);	
}

void s3c_wdt_mode(struct _wdt_t *w)
{
	if(!w->flag){	//reset
		w->init_reset(w);			
	}else{		//timer
		w->init_timer(w);	
	}
}
void s3c_wdt_timer_start(struct _wdt_t *w)
{
	iowrite32(0x8024, w->wtcon);	
}

void s3c_wdt_timer_stop(struct _wdt_t *w)
{
	iowrite32(0x8020, w->wtcon);	
}
void s3c_wdt_init_timer(struct _wdt_t *w)
{
	iowrite32(0x8020, w->wtcon);	
	iowrite32(0x8000, w->wtdat);
	iowrite32(0x8000, w->wtcnt);
}

void s3c_wdt_init_reset(struct _wdt_t *w)
{
	iowrite32(0x8021, w->wtcon);	
	iowrite32(0x8000, w->wtdat);
	iowrite32(0x8000, w->wtcnt);
}

/* 中断处理函数 */
irqreturn_t wdt_irq_handler(int irq, void *dev_id)
{
	printk("wang wang wang ...\n");
	return IRQ_HANDLED;	
}

/****************
 * Adaptor/Init
 ****************/
static int init_wdt_device(struct _wdt_t *w)
{
	int ret;
	w->phys = 0x53000000;	
	w->virt = ioremap(w->phys, SZ_4K);

	w->wtcon = w->virt + 0x00;
	w->wtdat = w->virt + 0x04;
	w->wtcnt = w->virt + 0x08;
	
	w->init_reset = s3c_wdt_init_reset;
	w->init_timer = s3c_wdt_init_timer;
	w->timer_start = s3c_wdt_timer_start;
	w->timer_stop = s3c_wdt_timer_stop;
	w->mode	= s3c_wdt_mode;
	w->feed	= s3c_wdt_feed;

	//register irq
	ret = request_irq(IRQ_S3C2440_WDT, wdt_irq_handler, IRQF_TRIGGER_NONE, "wdt-irq", NULL);		
	if(ret)
	{
		printk("wdt irq failed\n");
		ret = -EBUSY;
		goto err;
	}

	//init
	w->flag = 0;
	sema_init(&w->sem, 1);

	return 0;
err:
	iounmap(w->virt);
	return ret;

}

static void destroy_wdt_device(struct _wdt_t *w)
{
	free_irq(IRQ_S3C2440_WDT, NULL);
	iounmap(w->virt);
}

struct _wdt_t *wdt;
/**********
 * Char
 *********/
int char_wdt_ioctl (struct inode *node, struct file *filp, unsigned int cmd, unsigned long args)
{
	int ret;
	if(down_interruptible(&wdt->sem))
		return -ERESTARTSYS;

	switch(cmd)
	{
		case WDT_MODE:
			wdt->flag = args;
			wdt->mode(wdt);
			break;
		case WDT_FEED:
			wdt->feed(wdt);
			break;
		case WDT_START:
			wdt->timer_start(wdt);
			break;
		case WDT_STOP:
			wdt->timer_stop(wdt);
			break;

		default:
			printk("unknow wdt ioctl cmd\n");
			ret = -EINVAL;
			goto err;

	}
	up(&wdt->sem);
	return 0;

err:
	up(&wdt->sem);
	return ret;
}
struct file_operations wdt_fops = {
	.ioctl = char_wdt_ioctl,
};

/***************
 * Misc
 *************/
struct miscdevice misc_wdt = {
	.name = "wdt",
	.fops = &wdt_fops,
};

/***********
 * Plat Bus
 **********/
int wdt_probe(struct platform_device *pdev)
{
	printk("%s:entry\n", __FUNCTION__);
	int ret;
	wdt = kmalloc(sizeof(struct _wdt_t), GFP_KERNEL);	
	if(NULL == wdt)
	{
		printk("kmalloc wdt failed\n");
		ret = -ENOMEM;
		goto err;
	}

	//init
	init_wdt_device(wdt);	

	//test
	//wdt->init_reset(wdt);		
	//wdt->init_timer(wdt);
	//wdt->timer_start(wdt);

	//misc + char register
	misc_register(&misc_wdt);
		
	return 0;

err:
	return ret;
}

struct platform_driver plat_wdt = {
	.probe = wdt_probe,	
	.driver = {
		.name = "s3c2440a-wdt"
	},
};

/**************
 * Module
 *************/
static int wdt_driver_init(void)
{
	platform_driver_register(&plat_wdt);
	return 0;
}

static void wdt_driver_exit(void)
{
	//misc
	misc_deregister(&misc_wdt);

	//plat
	platform_driver_unregister(&plat_wdt);

	//wdt->timer_stop(wdt);
	//destroy
	destroy_wdt_device(wdt);
	kfree(wdt);
	printk("[%s]\n", __FUNCTION__);
}

module_init(wdt_driver_init);
module_exit(wdt_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


