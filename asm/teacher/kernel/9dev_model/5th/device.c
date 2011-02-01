#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include "usb_bus.h"

void mouse_release(struct device *dev)
{
		
}

struct usb_device mouse_dev = {
	.VendorID = 0x1122,
	.ProductID = 0x3344,
	.dev = {
		.bus_id = "test-mouse-device",	
		.release = mouse_release,
	},
};

int usb_device_init(void)
{
	usb_device_register(&mouse_dev);	
	printk("hello bus\n");
	return 0;
}

void usb_device_exit(void)
{
	usb_device_unregister(&mouse_dev);
	printk("[%s]\n", __FUNCTION__);
}

module_init(usb_device_init);
module_exit(usb_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


