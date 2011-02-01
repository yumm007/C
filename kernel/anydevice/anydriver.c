#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#define REGISTER_DRIVER(DEVICE_NAME) \
\
static int DEVICE_NAME##_device_init(struct platform_device *dev) { \
	printk(KERN_WARNING "%s\n", __FUNCTION__); \
	return 0;\
};\
\
static int DEVICE_NAME##_device_destroy(struct platform_device *dev) {\
	return 0;\
};\
\
static struct platform_driver drv = { 	\
	.probe	= DEVICE_NAME##_device_init,			\
	.remove	= DEVICE_NAME##_device_destroy,		\
	.driver.name = #DEVICE_NAME,					\
};													\
\
int DEVICE_NAME##_driver_init(void) {				\
	return platform_driver_register(&drv);	\
}													\
\
void DEVICE_NAME##_driver_exit(void) {				\
}													\
\
module_init(DEVICE_NAME##_driver_init);				\
module_exit(DEVICE_NAME##_driver_exit);

REGISTER_DRIVER(any);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
