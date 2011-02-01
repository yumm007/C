#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

extern struct bus_type usb_bus;

struct device_driver mouse_drv = {
	.name = "mouse",
	.bus = &usb_bus,
};

int usb_driver_init(void)
{
	driver_register(&mouse_drv);	
	printk("hello bus\n");
	return 0;
}

void usb_driver_exit(void)
{
	driver_unregister(&mouse_drv);
	printk("[%s]\n", __FUNCTION__);
}

module_init(usb_driver_init);
module_exit(usb_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


