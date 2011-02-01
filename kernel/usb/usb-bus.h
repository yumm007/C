#ifndef USB_BUS_H
#define USB_BUS_H

#include <linux/device.h>

struct test_usb_device {
	unsigned long address;
	unsigned long irq;
	unsigned long VenderID;
	unsigned long ProductID;
	struct device dev;
};	

struct test_usb_driver {
	unsigned long VenderID;
	unsigned long ProductID;
	struct device_driver drv;
};

int usb_device_register(struct test_usb_device *);
void usb_device_unregister(struct test_usb_device *);
int usb_driver_register(struct test_usb_driver *);
void usb_driver_unregister(struct test_usb_driver *);

#endif

