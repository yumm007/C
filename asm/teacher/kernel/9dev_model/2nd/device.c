#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

extern struct bus_type usb_bus;

void mouse_release(struct device *dev)
{
		
}

struct device mouse_dev = {
	.bus_id = "mouse",	
	.release = mouse_release,
	.bus = &usb_bus,
};

int usb_device_init(void)
{
	device_register(&mouse_dev);	
	printk("hello bus\n");
	return 0;
}

void usb_device_exit(void)
{
	device_unregister(&mouse_dev);
	printk("[%s]\n", __FUNCTION__);
}

module_init(usb_device_init);
module_exit(usb_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


