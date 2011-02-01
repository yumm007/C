#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

extern struct bus_type usb_bus;

static void usb_release(struct device *dev) {

}

struct device usb_mouse = {
	.init_name	= "usb-mouse-test",
	.bus			= &usb_bus,
	.release		= usb_release,
};

int usb_mouse_init(void) {
	device_register(&usb_mouse);
	return 0;
}

void usb_mouse_exit(void) {
	device_unregister(&usb_mouse);
}

module_init(usb_mouse_init);
module_exit(usb_mouse_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
