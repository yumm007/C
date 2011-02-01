#ifndef	_BUS_H
#define	_BUS_H

struct usb_device{
	unsigned long address, irq;
	unsigned long VendorID, ProductID;

	struct device dev;
};

struct usb_driver{
	unsigned long VendorID, ProductID;
	struct device_driver drv;
};

int usb_device_register(struct usb_device *udev);
void usb_device_unregister(struct usb_device *udev);
int usb_driver_register(struct usb_driver *udrv);
void usb_driver_unregister(struct usb_driver *udrv);

#endif	/* _BUS_H */
