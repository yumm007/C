#include <linux/module.h>
#include <linux/init.h>

#include "usb-bus.h"

static int test_usb_bus_match(struct device *dev, struct device_driver *drv) {
	struct test_usb_device *dev1 = container_of(dev, struct test_usb_device, dev);
	struct test_usb_driver *drv1 = container_of(drv, struct test_usb_driver, drv);

	return !(dev1->VenderID != drv1->VenderID || \
		dev1->ProductID != drv1->ProductID);
}

struct bus_type test_usb_bus = {
	.name = "test_usb_bus",
	.match = test_usb_bus_match,
};

int test_usb_bus_init(void) {
	return bus_register(&test_usb_bus);
}

void test_usb_bus_exit(void) {
	bus_unregister(&test_usb_bus);
}

int usb_device_register(struct test_usb_device *dev) {
	dev->dev.bus = &test_usb_bus;
	return device_register(&dev->dev);
}

void usb_device_unregister(struct test_usb_device *dev) {
	device_unregister(&dev->dev);
}

int usb_driver_register(struct test_usb_driver *drv) {
	drv->drv.bus = &test_usb_bus;
	return driver_register(&drv->drv);
}
void usb_driver_unregister(struct test_usb_driver *drv) {
	driver_unregister(&drv->drv);
}

module_init(test_usb_bus_init);
module_exit(test_usb_bus_exit);

EXPORT_SYMBOL(usb_device_register);
EXPORT_SYMBOL(usb_device_unregister);
EXPORT_SYMBOL(usb_driver_register);
EXPORT_SYMBOL(usb_driver_unregister);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
