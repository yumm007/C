#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

extern struct bus_type usb_bus;
int usb_match(struct device *dev, struct device_driver *drv) {
	return ((dev->bus == &usb_bus) && \
			(!strcmp(drv->name, "usb-mouse-testdriver")));
}

struct bus_type usb_bus = {
	.name = "usb-bus",
	.match = usb_match,
};
EXPORT_SYMBOL(usb_bus);

int usb_bus_init(void) {
	bus_register(&usb_bus);

	return 0;
}

void usb_bus_exit(void) {
	bus_unregister(&usb_bus);
}

module_init(usb_bus_init);
module_exit(usb_bus_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
