#include <linux/module.h>
#include <linux/init.h>
#include "usb-bus.h"

static void test_usb_device_release(struct device *dev) {
}

static struct test_usb_device dev = {
	.VenderID	= 0x1111,
	.ProductID	= 0x2222,
	.dev.init_name	= "test_usb_device",
	.dev.release	= test_usb_device_release,
};

int test_usb_device_init(void) {
	return usb_device_register(&dev);
}

void test_usb_device_exit(void) {
	usb_device_unregister(&dev);
}

module_init(test_usb_device_init);
module_exit(test_usb_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
