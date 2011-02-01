#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/input.h>

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
	struct input_dev *dev;

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
static char *s3c2440ats_name = "s3c2440A liy TouchScreen";

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
	input_report_abs(t->dev, ABS_X, t->x);
	input_report_abs(t->dev, ABS_Y, t->y);
	input_sync(t->dev);
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
		input_report_abs(t->dev, ABS_PRESSURE, 1);
		t->auto_xy(t);	
		t->adc_start(t);
	}else{
		input_report_abs(t->dev, ABS_PRESSURE, 0);
		input_sync(t->dev);
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

/*****
 * Input
 *****/
int register_input_subsystem(struct _ts_t *t)
{
	 /* Initialise input 对象 */
	//分配空间
	t->dev = input_allocate_device();

	//支持什么事件
	t->dev->evbit[0] = BIT(EV_SYN) | BIT(EV_KEY) | BIT(EV_ABS);
	t->dev->keybit[BITS_TO_LONGS(BTN_TOUCH)] = BIT(BTN_TOUCH);

	//初始化事件值范围
	input_set_abs_params(t->dev, ABS_X, 0, 0x3FF, 0, 0);
	input_set_abs_params(t->dev, ABS_Y, 0, 0x3FF, 0, 0);
	input_set_abs_params(t->dev, ABS_PRESSURE, 0, 1, 0, 0);

	//属性信息
	t->dev->name = s3c2440ats_name;
        t->dev->id.vendor = 0xABCD;
        t->dev->id.product = 0xABCD;
        t->dev->id.version = 0x1;

	/* All went ok, so register to the input system */
	input_register_device(t->dev);
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

	//init input subsystem
	register_input_subsystem(t);

	return 0;
err1:
	free_irq(t->int_adc, t);
err:
	iounmap(t->virt);
	return ret;
}

void destroy_ts_device(struct _ts_t *t)
{
	input_unregister_device(t->dev);
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


