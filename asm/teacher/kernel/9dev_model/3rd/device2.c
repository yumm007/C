#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

extern struct bus_type usb_bus;

void mouse_release2(struct device *dev)
{
		
}

struct device mouse_dev2 = {
	.bus_id = "mouse2",	
	.release = mouse_release2,
	.bus = &usb_bus,
};

int usb_device_init(void)
{
	device_register(&mouse_dev2);	
	printk("hello bus\n");
	return 0;
}

void usb_device_exit(void)
{
	device_unregister(&mouse_dev2);
	printk("[%s]\n", __FUNCTION__);
}

module_init(usb_device_init);
module_exit(usb_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


