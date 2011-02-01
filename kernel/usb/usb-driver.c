#include <linux/module.h>
#include <linux/init.h>
#include "usb-bus.h"

static int test_usb_driver_probe(struct device *dev) {
	printk("init usb device...\n");
	return 0;
}

static struct test_usb_driver drv = {
	.VenderID	= 0x1111,
	.ProductID	= 0x2222,
	.drv.name	= "test-usb-driver",
	.drv.probe	= test_usb_driver_probe,
};

int test_usb_driver_init(void) {
	return usb_driver_register(&drv);
}

void test_usb_driver_exit(void) {
	usb_driver_unregister(&drv);
}

module_init(test_usb_driver_init);
module_exit(test_usb_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
