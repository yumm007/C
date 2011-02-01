#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>

int test_init(void)
{
#if 0
	struct task_struct *task;
	task = get_current();
	printk("%s %d %s\n", __FUNCTION__, task->pid, task->comm);
#else
	printk("%s [%d] [%s]\n", __FUNCTION__, current->pid, current->comm);
#endif
	return 0;
}

void test_exit(void)
{
	printk("%s\n", __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);
