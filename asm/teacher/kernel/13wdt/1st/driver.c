#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/interrupt.h>

#include <asm/io.h>
#include <asm/sizes.h>

struct _wdt_t {
	unsigned long phys, virt;
	unsigned long wtcon, wtdat, wtcnt;

	void (*init_reset)(struct _wdt_t *);
	void (*init_timer)(struct _wdt_t *);
	void (*timer_start)(struct _wdt_t *);
	void (*timer_stop)(struct _wdt_t *);

};

/*******
 * Arm
 ********/
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

	//register irq
	ret = request_irq(IRQ_S3C2440_WDT, wdt_irq_handler, IRQF_TRIGGER_NONE, "wdt-irq", NULL);		
	if(ret)
	{
		printk("wdt irq failed\n");
		ret = -EBUSY;
		goto err;
	}

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
/**************
 * Module
 *************/
static int wdt_driver_init(void)
{
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
	wdt->init_timer(wdt);
	wdt->timer_start(wdt);

	return 0;

err:
	return ret;
}

static void wdt_driver_exit(void)
{
	wdt->timer_stop(wdt);
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


