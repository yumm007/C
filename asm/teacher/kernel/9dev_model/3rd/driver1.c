#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

extern struct bus_type usb_bus;

int mouse_probe2 (struct device *dev)
{
	printk("[%s]entry\n", __FUNCTION__);
	return 0;	
}
struct device_driver mouse_drv2 = {
	.name = "mouse2",
	.bus = &usb_bus,
	.probe = mouse_probe2,
};

int usb_driver_init(void)
{
	driver_register(&mouse_drv2);	
	printk("hello bus\n");
	return 0;
}

void usb_driver_exit(void)
{
	driver_unregister(&mouse_drv2);
	printk("[%s]\n", __FUNCTION__);
}

module_init(usb_driver_init);
module_exit(usb_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


