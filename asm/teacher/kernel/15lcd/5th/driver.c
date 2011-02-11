#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/interrupt.h>

#include <asm/io.h>
#include <asm/sizes.h>
#include <asm/uaccess.h>

#include "lcd.h"

/* lcdcon1 */
#define	CLKVAL(x)	((x) << 8)
#define	PNRMODE(x)	((x) << 5)
#define	BPPMODE(x)	((x) << 1)
#define	ENVID(x)	((x) << 0)

/* lcdcon2 */
#define	VBPD(x)		((x) << 24)
#define	LINEVAL(x)	((x) << 14)
#define	VFPD(x)		((x) << 6)
#define	VSPW(x)		((x) << 0)

/* lcdcon3 */
#define	HBPD(x)		((x) << 19)
#define	HOZVAL(x)	((x) << 8)
#define	HFPD(x)		((x) << 0)

/* lcdcon4 */
#define	HSPW(x)		((x) << 0)

/* lcdcon5 */
#define	FRM565(x)	((x) << 11)
#define	INVVCLK(x)	((x) << 10)
#define	INVVLINE(x)	((x) << 9)
#define	INVVFRAME(x)	((x) << 8)
#define	INVPWREN(x)	((x) << 5)
#define	PWREN(x)	((x) << 3)
#define	HWSWP(x)	((x) << 0)

/* lcdsaddr1 */
#define	LCDBANK(x)	((x) << 21)
#define	LCDBASEU(x)	((x) << 0)
/* lcdsaddr2 */
#define	LCDBASEL(x)	((x) << 0)
/* lcdsaddr3 */
#define	OFFSIZE(x)	((x) << 11)
#define	PAGEWIDTH(x)	((x) << 0)

#define	RGB(r, g, b)	((r << 11) | ((g >> 2) << 5) | (b >> 3))
	
struct _lcd_t{
	unsigned long phys, virt;
	unsigned long lcdcon1, lcdcon2, lcdcon3, lcdcon4, lcdcon5;
	unsigned long lcdsaddr1, lcdsaddr2, lcdsaddr3;

	unsigned long gpio_phys, gpio_virt;
	unsigned long gpbcon, gpbup, gpccon, gpcup, gpdcon, gpdup;	

	unsigned long fb_virt, fb_phys, fb_size;
	int irq;
	struct fasync_struct *fa;

	void (*config)(struct _lcd_t *l);
	void (*enable)(struct _lcd_t *l);
	void (*disable)(struct _lcd_t *l);

};

static void s3c_lcd_enable(struct _lcd_t *l)
{
	iowrite32(ioread32(l->lcdcon1) | ENVID(1), l->lcdcon1);
	iowrite32(ioread32(l->lcdcon5) | PWREN(1), l->lcdcon5);
}

static void s3c_lcd_disable(struct _lcd_t *l)
{
	iowrite32(ioread32(l->lcdcon1) & ~ENVID(1), l->lcdcon1);
	iowrite32(ioread32(l->lcdcon5) & ~PWREN(1), l->lcdcon5);
}

static void s3c_lcd_config(struct _lcd_t *l)
{
	unsigned int reg;
	/* config gpio for lcd pin */
	reg = ioread32(l->gpbcon);
	reg &= ~(3 << 0);
	reg |= (1 << 0);
	iowrite32(reg, l->gpbcon);
	iowrite32(0xf, l->gpbup);

	iowrite32(0xaaaaaaaa, l->gpccon);
	iowrite32(0xffff, l->gpcup);

	iowrite32(0xaaaaaaaa, l->gpdcon);
	iowrite32(0xffff, l->gpdup);

	/* lcdcon1 */	
	iowrite32(CLKVAL(5) | PNRMODE(3) | BPPMODE(0xc), l->lcdcon1);
	/* lcdcon2 */
	iowrite32(VBPD(10) | LINEVAL(271) | VFPD(2) | VSPW(10), l->lcdcon2);
	/* lcdcon3 */
	iowrite32(HBPD(2) | HOZVAL(479) | HFPD(2), l->lcdcon3);
	/* lcdcon4 */
	iowrite32(HSPW(41), l->lcdcon4);
	/* lcdcon5 */
	iowrite32(FRM565(1) | INVVLINE(1) | INVVFRAME(1) | INVPWREN(1) | HWSWP(1), l->lcdcon5);

	/* lcdsaddr1*/
	iowrite32(LCDBANK(l->fb_phys >> 22) | (LCDBASEU((l->fb_phys >> 1) & 0x1fffff)), l->lcdsaddr1);
	/* lcdsaddr2*/
	iowrite32(LCDBASEL(((l->fb_phys >> 1) & 0x1fffff) + 480*272), l->lcdsaddr2);
	/* lcdsaddr3*/
	iowrite32(OFFSIZE(0) | PAGEWIDTH(480), l->lcdsaddr3);

}

irqreturn_t irq_lcd_handler(int irq, void *dev_id)
{
	struct _lcd_t *l = (struct _lcd_t *)dev_id;
	printk("#\n");
	kill_fasync(&l->fa, SIGIO, POLL_IN);
	return IRQ_HANDLED;	
}

int lcd_device_init(struct _lcd_t *l)
{
	int ret;
	l->phys = 0X4D000000;
	l->virt = ioremap(l->phys, SZ_4K);
	if(NULL == l->virt)
	{
		printk("iomap lcd virt failed\n");
		ret = -EBUSY;
		goto err;
	}
	l->lcdcon1 = l->virt + 0x00;
	l->lcdcon2 = l->virt + 0x04;
	l->lcdcon3 = l->virt + 0x08;
	l->lcdcon4 = l->virt + 0x0c;
	l->lcdcon5 = l->virt + 0x10;
	l->lcdsaddr1 = l->virt + 0x14;
	l->lcdsaddr2 = l->virt + 0x18;
	l->lcdsaddr3 = l->virt + 0x1c;

	l->gpio_phys = 0x56000000;
	l->gpio_virt = ioremap(l->gpio_phys, SZ_4K);
	if(NULL == l->gpio_virt)
	{
		printk("iomap gpio virt failed\n");
		ret = -EBUSY;
		goto err1;
	}
	l->gpbcon = l->gpio_virt + 0x10;
	l->gpbup = l->gpio_virt + 0x18;
	l->gpccon = l->gpio_virt + 0x20;
	l->gpcup = l->gpio_virt + 0x28;
	l->gpdcon = l->gpio_virt + 0x30;
	l->gpdup = l->gpio_virt + 0x38;

	l->fb_size = 272*480*2;
	l->fb_virt = dma_alloc_writecombine(NULL, l->fb_size, &l->fb_phys, GFP_KERNEL);
	if(NULL == l->gpio_virt)
	{
		printk("dma alloc failed\n");
		ret = -ENOMEM;
		goto err2;
	}

	l->config = s3c_lcd_config;
	l->enable = s3c_lcd_enable;
	l->disable = s3c_lcd_disable;

	request_irq(IRQ_EINT1, irq_lcd_handler, IRQF_TRIGGER_FALLING, "lcd irq", (void *)l);

	return 0;
err2:
	iounmap(l->gpio_virt);
err1:
	iounmap(l->virt);

err:
	return ret;
}

void lcd_device_destory(struct _lcd_t *l)
{
	free_irq(IRQ_EINT1, l);
	dma_free_coherent(NULL, l->fb_size, l->fb_virt, l->fb_phys);
	iounmap(l->gpio_virt);
	iounmap(l->virt);
}

void test_lcd(struct _lcd_t *l)
{
	unsigned short (*p)[480];
	int i, j;
	l->config(l);		
	l->enable(l);
	
	/* fill memory(framebuffer) */
	p = (unsigned short *)l->fb_virt;	
	for(i = 0; i < 272; i++)
	{
		for(j = 0; j < 480; j++)
		{
			p[i][j] = RGB(255, 0, 0);	
		}
	}
}

struct _lcd_t *lcd;
int s3c_lcd_ioctl (struct inode *node, struct file *filp, unsigned int cmd, unsigned long args)
{
	switch(cmd){
		case CONFIG_LCD:
			lcd->config(lcd);
			break;
		case ENABLE_LCD:
			lcd->enable(lcd);
			break;
		case DISABLE_LCD:
			lcd->disable(lcd);
			break;
		default:
			printk("unknow ioctl cmd\n");
	};
	return 0;
}

ssize_t s3c_lcd_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	int ret;
	ret = copy_from_user(lcd->fb_virt, buf, count);
	return (count - ret);
}

int s3c_lcd_fasync(int fd, struct file *filp, int on)
{
	return fasync_helper(fd, filp, on, &lcd->fa);	
}

struct file_operations lcd_fops = {
	.ioctl = s3c_lcd_ioctl,
	.write = s3c_lcd_write,
	.fasync = s3c_lcd_fasync,
};

struct miscdevice misc_lcd = {
	.name = "lcd",	
	.fops = &lcd_fops,
};

int s3c_lcd_probe(struct platform_device *pdev)
{
	printk("%s\n", __FUNCTION__);
	int ret;
	lcd = kmalloc(sizeof(struct _lcd_t), GFP_KERNEL);					
	if(NULL == lcd)
	{
		printk("lcd alloc failed\n");
		ret = -ENOMEM;
		goto err;
	}
	printk("hello lcd\n");
	ret = lcd_device_init(lcd);
	if(ret){
		printk("lcd device init failed\n");
		ret = -EBUSY;
		goto err1;
	}
	
	misc_register(&misc_lcd);
	

	//test_lcd(lcd);
	
	return 0;

err1:
	kfree(lcd);
err:
	return ret;
}

int s3c_lcd_remove(struct platform_device *pdev)
{
	misc_deregister(&misc_lcd);
	lcd_device_destory(lcd);
	kfree(lcd);
	return 0;	
}

struct platform_driver plcd_drv = {
	.probe = s3c_lcd_probe,	
	.remove = s3c_lcd_remove,
	.driver = {
		.name = "s3c2440a-lcd",
	},
};

int lcd_driver_init(void)
{
	platform_driver_register(&plcd_drv);
	return 0;
}

void lcd_driver_exit(void)
{
	platform_driver_unregister(&plcd_drv);
	printk("[%s]\n", __FUNCTION__);
}

module_init(lcd_driver_init);
module_exit(lcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


