#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

int usb_match(struct device *dev, struct device_driver *drv)
{
	if(!strcmp(dev->bus_id, drv->name)){
		printk("match success\n");
		return 1;
	}else{
		printk("match failed\n");
		return 0;
	}
}

struct bus_type	usb_bus = {
	.name = "usb-bus",	
	.match = usb_match,
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


