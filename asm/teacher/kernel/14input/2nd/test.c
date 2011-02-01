#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/timer.h>
#include <linux/workqueue.h>

#include <asm/io.h>
#include <asm/sizes.h>

struct _ts_t{
	unsigned long phys, virt;
	unsigned long adccon, adctsc, adcdat0, adcdat1, adcupdn;
	int int_adc;
	int int_tc;
	unsigned long x, y;
	struct work_struct work;
	struct timer_list timer;

	void (*config_adc)(struct _ts_t *t);
	void (*config_ts)(struct _ts_t *t);
	void (*adc_start)(struct _ts_t *t);
	void (*adc_stop)(struct _ts_t *t);
	void (*auto_xy)(struct _ts_t *t);
	unsigned long (*read_x)(struct _ts_t *t);
	unsigned long (*read_y)(struct _ts_t *t);
	void (*wait_up)(struct _ts_t *t);
	void (*wait_down)(struct _ts_t *t);

};

void s3c_wait_down(struct _ts_t *t)
{
	iowrite32(0xd3, t->adctsc);	
}

void s3c_wait_up(struct _ts_t *t)
{
	iowrite32(0x1d3, t->adctsc);	
}

unsigned long s3c_read_x(struct _ts_t *t)
{
	return (ioread32(t->adcdat0) & 0x3ff);	
}
unsigned long s3c_read_y(struct _ts_t *t)
{
	return (ioread32(t->adcdat1) & 0x3ff);	
}

void s3c_auto_xy(struct _ts_t *t)
{
	iowrite32(0xd7, t->adctsc);	
}

void s3c_adc_start(struct _ts_t *t)
{
	iowrite32(ioread32(t->adccon) | (1 << 0), t->adccon);		
}

void s3c_adc_stop(struct _ts_t *t)
{
	iowrite32(ioread32(t->adccon) & ~(1 << 0), t->adccon);		
}

void s3c_config_adc(struct _ts_t *t)
{
	int reg;
	reg = ioread32(t->adccon);
	reg = (1 << 14) | (49 << 6) | (0 << 2);
	iowrite32(reg, t->adccon);
}

void s3c_config_ts(struct _ts_t *t)
{
	//printk("before:0x%x\n", ioread32(t->adctsc));
	iowrite32(0xd3, t->adctsc);		
	//printk("after:0x%x\n", ioread32(t->adctsc));
}

irqreturn_t adc_irq_handle(int irq, void *dev_id)
{
	struct _ts_t *t = (struct _ts_t *)dev_id;
	t->x = t->read_x(t);
	t->y = t->read_y(t);
	printk("x:%d y:%d\n", t->x, t->y);
	t->wait_up(t);	

	return IRQ_HANDLED;	
}

irqreturn_t tc_irq_handle(int irq, void *dev_id)
{
	struct _ts_t *t = (struct _ts_t *)dev_id;
#if 1
	mod_timer(&t->timer, jiffies + 30);
#else
	del_timer(&t->timer);
	t->timer.expires = jiffies + 30;
	add_timer(&t->timer);
#endif
//	schedule_work(&t->work);
	return IRQ_HANDLED;	

}

void timer_function(unsigned long data)
{
	struct _ts_t *t = (struct _ts_t *)data;

	if(ioread32(t->adcupdn) & 0x1){
		printk("down\n");	
		t->auto_xy(t);	
		t->adc_start(t);
	}else{
		printk("up\n");	
		t->wait_down(t);	
	}
	iowrite32(0, t->adcupdn);

}

void work_fun(struct work_struct *work)
{
	struct _ts_t *t = container_of(work, struct _ts_t, work);
	t->timer.expires = jiffies + 30;
	add_timer(&t->timer);
}

int create_ts_device(struct _ts_t *t)
{
	int ret;
	t->phys = 0x58000000;
	t->virt = ioremap(t->phys, SZ_4K);
	
	t->adccon = t->virt + 0x00;
	t->adctsc = t->virt + 0x04;
	t->adcdat0 = t->virt + 0x0c;
	t->adcdat1 = t->virt + 0x10;
	t->adcupdn = t->virt + 0x14;

	t->config_adc = s3c_config_adc;
	t->config_ts = s3c_config_ts;
	t->adc_start = s3c_adc_start;
	t->adc_stop = s3c_adc_stop;
	t->auto_xy = s3c_auto_xy;
	t->read_x = s3c_read_x;
	t->read_y = s3c_read_y;
	t->wait_up = s3c_wait_up;
	t->wait_down = s3c_wait_down;

	//irq
	t->int_adc = IRQ_ADC;	
	t->int_tc = IRQ_TC;

	//clk
#if 0
	int reg;
	unsigned long clkcon;
	clkcon = ioremap(0x4C00000C, 4);

	reg = ioread32(clkcon);
	reg |= (1 << 15);
	iowrite32(reg, clkcon);
#else
	struct clk *adc_clk;
	adc_clk = clk_get(NULL, "adc");			
	clk_enable(adc_clk);	
#endif
	init_timer(&t->timer);
	t->timer.expires = jiffies + 30;
	t->timer.function = timer_function;
	t->timer.data = (unsigned long)t;
#if 0
	INIT_WORK(&t->work, work_fun);
#endif
	ret = request_irq(t->int_adc, adc_irq_handle, IRQF_TRIGGER_MASK, "test-adc-irq", t);	
	if(ret)
	{
		printk("adc irq failed\n");	
		ret = -EBUSY;
		goto err;
	}

	ret = request_irq(t->int_tc, tc_irq_handle, IRQF_TRIGGER_MASK, "test-tc-irq", t);	
	if(ret)
	{
		printk("tc irq failed\n");	
		ret = -EBUSY;
		goto err1;
	}

	return 0;
err1:
	free_irq(t->int_adc, t);
err:
	iounmap(t->virt);
	return ret;
}

void destroy_ts_device(struct _ts_t *t)
{
	free_irq(t->int_adc, t);
	free_irq(t->int_tc, t);
	iounmap(t->virt);
}

struct _ts_t *ts;
int ts_driver_init(void)
{
	int ret;
	ts = kmalloc(sizeof(struct _ts_t), GFP_KERNEL);	
	if(NULL == ts)
	{
		printk("kmalloc ts failed\n");
		ret = -ENOMEM;
		goto err;
	}
	create_ts_device(ts);

	ts->config_adc(ts);
	ts->config_ts(ts);

	return 0;
err:
	return ret;
}

void ts_driver_exit(void)
{
	destroy_ts_device(ts);
	kfree(ts);
	printk("[%s]\n", __FUNCTION__);
}

module_init(ts_driver_init);
module_exit(ts_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


