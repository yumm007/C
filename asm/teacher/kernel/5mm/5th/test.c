#include <linux/module.h>
#include <linux/init.h>

#include <linux/mm.h>

struct page *p;
int test_init(void)
{
	unsigned long virt;
	p = alloc_pages(GFP_KERNEL, 0);
	virt = page_address(p);
	memcpy(virt, "alloc one page", 15);
	printk("val:[%s] addr:[%p]\n", virt, virt);

	printk("hello mm\n");
	return 0;
}

void test_exit(void)
{
	__free_pages(p, 0);
	printk("[%s]\n", __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_VERSION("v0.1");


