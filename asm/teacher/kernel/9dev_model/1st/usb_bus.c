#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

struct bus_type	usb_bus = {
	.name = "usb-bus",	
};
EXPORT_SYMBOL(usb_bus);

int usb_bus_init(void)
{
	bus_register(&usb_bus);	
	printk("hello bus\n");
	return 0;
}

void usb_bus_exit(void)
{
	bus_unregister(&usb_bus);
	printk("[%s]\n", __FUNCTION__);
}

module_init(usb_bus_init);
module_exit(usb_bus_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


