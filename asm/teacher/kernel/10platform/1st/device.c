#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
	
void led_release(struct device *dev)
{
	                
}

struct resource led_res = {
	.start = 0x56000000,
	.end = 0x560000ff,
};

struct platform_device plat_dev = {
	.name = "s3c2440a-led",
	.id = -1,
	.dev = {
		.release = led_release,		
	},
	.num_resources = 1,
	.resource = &led_res,
};

int test_device_init(void)
{
	platform_device_register(&plat_dev);
	printk("hello plat device\n");
	return 0;
}

void test_device_exit(void)
{
	platform_device_unregister(&plat_dev);
	printk("[%s]\n", __FUNCTION__);
}

module_init(test_device_init);
module_exit(test_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


