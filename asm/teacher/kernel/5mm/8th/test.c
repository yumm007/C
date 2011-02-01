#include <linux/module.h>
#include <linux/init.h>

#include <linux/mm.h>
#include <linux/vmalloc.h>

int test_init(void)
{
	char *virt;
//	unsigned long phys;
	virt = vmalloc(20);
	memcpy(virt, "kmalloc page", 15);
	printk("val:[%s] virt addr is [0x%p]\n", virt, virt);
	//phys = __pa(virt);
	//printk("phys addr is [0x%p]\n", phys);

	vfree(virt);
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


