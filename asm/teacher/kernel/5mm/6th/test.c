#include <linux/module.h>
#include <linux/init.h>

#include <linux/mm.h>

void *virt;
int test_init(void)
{
	struct page *p;
	virt = __get_free_pages(GFP_KERNEL, 2);
	memcpy(virt, "alloc four page", 15);
	printk("val:[%s] addr:[%p]\n", virt, virt);

	printk("hello mm\n");
	return 0;
}

void test_exit(void)
{
	printk("[%s]\n", __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


