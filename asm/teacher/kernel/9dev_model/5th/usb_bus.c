#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

#include "usb_bus.h"

int usb_match(struct device *dev, struct device_driver *drv)
{
	struct usb_device *udev = container_of(dev, struct usb_device, dev);
	struct usb_driver *udrv = container_of(drv, struct usb_driver, drv);
	if((udev->VendorID == udrv->VendorID) && 
			(udev->ProductID == udrv->ProductID)){
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

int usb_device_register(struct usb_device *udev)
{
	udev->dev.bus = &usb_bus;
	return device_register(&udev->dev);
}

void usb_device_unregister(struct usb_device *udev)
{
	device_unregister(&udev->dev);
}

int usb_driver_register(struct usb_driver *udrv)
{
	udrv->drv.bus = &usb_bus;
	return driver_register(&udrv->drv);
}

void usb_driver_unregister(struct usb_driver *udrv)
{
	driver_unregister(&udrv->drv);
}

EXPORT_SYMBOL(usb_device_register);
EXPORT_SYMBOL(usb_device_unregister);
EXPORT_SYMBOL(usb_driver_register);
EXPORT_SYMBOL(usb_driver_unregister);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


