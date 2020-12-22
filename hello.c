#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>

#include <linux/ktime.h>
#include <linux/slab.h>

struct head_list {
	struct head_list *next;
	ktime_t time;
};

MODULE_AUTHOR("Ivan Fedorusov <i.fedorusov-2024@kpi.ua>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static struct head_list *head;

static uint counter = 1;
module_param(counter, uint, 0444);
MODULE_PARM_DESC(counter, "This parameter is for number of hello world");

static int __init hello_init(void)
{
	uint i = 0;

	struct head_list *this_var, *next_var;

	pr_info("Input %d\n", counter);

	if (!counter) {
		pr_info("You cannot enter 0");
		pr_info("");
		return 0;
	}

	if (counter >= 5 && counter <= 10) {
		pr_warn("You should enter number less than 5");
	} else {
		if (counter > 10) {
			pr_err("The parameter is greater than 10");
			pr_info("");
			return -EINVAL;
		}
	}

	head = kmalloc(sizeof(struct head_list *), GFP_KERNEL);
	this_var = head;

	for (i = 0; i < counter; i++) {
		this_var->next = kmalloc(sizeof(struct head_list), GFP_KERNEL);
		this_var->time = ktime_get();
		pr_info("Hello, world!\n");
		next_var = this_var;
		this_var = this_var->next;
	}

	kfree(next_var->next);
	next_var->next = NULL;

	return 0;
}

static void __exit hello_exit(void)
{
	struct head_list *tmp_var;

	while (head != NULL && counter != 0) {
		tmp_var = head;
		pr_info("Time: %lld", tmp_var->time);
		head = tmp_var->next;
		kfree(tmp_var);
	}
	pr_info("Module removed");
	pr_info("");
}

module_init(hello_init);
module_exit(hello_exit);
