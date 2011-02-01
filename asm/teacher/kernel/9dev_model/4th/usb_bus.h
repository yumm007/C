#ifndef	_BUS_H
#define	_BUS_H

int usb_device_register(struct device *dev);
void usb_device_unregister(struct device *dev);
int usb_driver_register(struct device_driver *drv);
void usb_driver_unregister(struct device_driver *drv);

#endif	/* _BUS_H */
