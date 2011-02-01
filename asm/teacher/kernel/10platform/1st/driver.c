#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
	
int plat_led_probe(struct platform_device *pdev)
{
	printk("%s\n", __FUNCTION__);	
	return 0;
}
struct platform_driver plat_drv = {
	.probe = plat_led_probe,
	.driver = {
		.name = "s3c2440a-led",	
	},	
};

int test_driver_init(void)
{
	platform_driver_register(&plat_drv);
	printk("hello plat driver\n");
	return 0;
}

void test_driver_exit(void)
{
	platform_driver_unregister(&plat_drv);
	printk("[%s]\n", __FUNCTION__);
}

module_init(test_driver_init);
module_exit(test_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


