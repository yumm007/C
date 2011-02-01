#include <linux/module.h>
#include <linux/init.h>

int testchar_init(void) {
}

void testchar_exit(void) {
}

module_init(testchar_init);
module_exit(testchar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YU");
MODULE_VERSION("v0.5");
