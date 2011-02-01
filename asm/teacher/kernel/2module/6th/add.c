#include <linux/module.h>
#include <linux/init.h>

#include "add.h"

int add_test1(int x, int y)
{
	return (x + y);
}

EXPORT_SYMBOL(add_test1);
