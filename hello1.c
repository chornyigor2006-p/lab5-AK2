// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include "hello1.h"

MODULE_AUTHOR("Ігор Чорний");
MODULE_DESCRIPTION("Module hello1: exports print_hello and tracks execution time");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_event {
	struct list_head list;
	ktime_t time_before;
	ktime_t time_after;
};

static LIST_HEAD(hello_list);

int print_hello(void)
{
	struct hello_event *event;

	event = kmalloc(sizeof(*event), GFP_KERNEL);
	if (!event) {
		pr_err("hello1: Memory allocation failed\n");
		return -ENOMEM;
	}

	event->time_before = ktime_get();
	pr_info("Hello, world!\n");
	event->time_after = ktime_get();

	list_add_tail(&event->list, &hello_list);

	return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1: Module loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_event *cursor, *tmp;
	s64 duration;

	list_for_each_entry_safe(cursor, tmp, &hello_list, list) {
		duration = ktime_to_ns(ktime_sub(cursor->time_after, cursor->time_before));
		pr_info("hello1: Print duration: %lld ns\n", duration);
		list_del(&cursor->list);
		kfree(cursor);
	}
	pr_info("hello1: Module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
