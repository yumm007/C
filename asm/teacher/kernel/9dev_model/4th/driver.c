#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include "usb_bus.h"
int mouse_probe (struct device *dev)
{
	printk("[%s]entry\n", __FUNCTION__);
	return 0;	
}
int mouse_remove(struct device *dev)
{
	printk("[%s]entry\n", __FUNCTION__);
	return 0;	
	
}
struct device_driver mouse_drv = {
	.name = "mouse",
	.probe = mouse_probe,
	.remove = mouse_remove,
};

int usb_driver_init(void)
{
	usb_driver_register(&mouse_drv);	
	printk("hello bus\n");
	return 0;
}

void usb_driver_exit(void)
{
	usb_driver_unregister(&mouse_drv);
	printk("[%s]\n", __FUNCTION__);
}

module_init(usb_driver_init);
module_exit(usb_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


