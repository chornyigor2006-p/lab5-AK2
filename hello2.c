// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include "hello1.h"

MODULE_AUTHOR("Ігор Чорний");
MODULE_DESCRIPTION("Module hello2: calls print_hello based on parameter");
MODULE_LICENSE("Dual BSD/GPL");

static uint hello_count = 1;
module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "Number of times to call print_hello()");

static int __init hello2_init(void)
{
	uint i;
	int ret;

	if (hello_count > 10) {
		pr_err("hello2: Parameter hello_count is too large (> 10)\n");
		return -EINVAL;
	}

	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10))
		pr_warn("hello2: Warning: hello_count is %u\n", hello_count);

	for (i = 0; i < hello_count; i++) {
		ret = print_hello();
		if (ret)
			return ret;
	}

	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2: Module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
