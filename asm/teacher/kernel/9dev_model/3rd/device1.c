#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

extern struct bus_type usb_bus;

void mouse_release1(struct device *dev)
{
		
}

struct device mouse_dev1 = {
	.bus_id = "mouse1",	
	.release = mouse_release1,
	.bus = &usb_bus,
};

int usb_device_init(void)
{
	device_register(&mouse_dev1);	
	printk("hello bus\n");
	return 0;
}

void usb_device_exit(void)
{
	device_unregister(&mouse_dev1);
	printk("[%s]\n", __FUNCTION__);
}

module_init(usb_device_init);
module_exit(usb_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


