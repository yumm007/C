#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

extern struct bus_type usb_bus;

int usb_mouse_test_probe(struct device *dev) {
	printk(KERN_WARNING "init...\n");
	return 0;
}

int usb_mouse_test_remove(struct device *dev) {
	return 0;
}

struct device_driver usb_driver = {
	.name	 =	"usb-mouse-testdriver",
	.bus	 = &usb_bus,
	.probe = usb_mouse_test_probe,
	.remove= usb_mouse_test_remove,
};

int usb_mouse_driver_init(void) {
	driver_register(&usb_driver);
	return 0;
}

void usb_mouse_driver_exit(void) {
	driver_unregister(&usb_driver);
}

module_init(usb_mouse_driver_init);
module_exit(usb_mouse_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
