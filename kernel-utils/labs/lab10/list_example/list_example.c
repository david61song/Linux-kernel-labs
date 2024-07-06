#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>	

#include <linux/slab.h>
#include <linux/list.h>

struct pid_list {
	pid_t pid;
	struct list_head list;
};

LIST_HEAD(my_list);

static int add_pid(pid_t pid)
{
	struct pid_list *ple = kmalloc(sizeof *ple, GFP_KERNEL);

	if (!ple)
		return -ENOMEM;

	ple->pid = pid;
	list_add(&ple->list, &my_list);

	return 0;
}

static int del_pid(pid_t pid)
{
	struct list_head *i, *tmp;
	struct pid_list *ple;

	list_for_each_safe(i, tmp, &my_list) {
		ple = list_entry(i, struct pid_list, list);
		if (ple->pid == pid) {
			list_del(i);
			kfree(ple);
			return 0;
		}
	}

	return -EINVAL;
}

static void destroy_list(void)
{
	struct list_head *i, *n;
	struct pid_list *ple;

	list_for_each_safe(i, n, &my_list) {
		ple = list_entry(i, struct pid_list, list);
		list_del(i);
		kfree(ple);
	}
	printk(KERN_INFO "Destroy list....\n");
}

static int __init list_module_init(void) 
{
	/*
	pid_t pids[10];
	int j = 0;
	struct list_head *i, *tmp;
	struct pid_list *entry;

	for (j = 0; j < 10; j++) {
		pids[j] = j * 10000 + j;
		add_pid(pids[j]);
	}

	printk(KERN_INFO "Create pid lists...\n");
	list_for_each_safe(i, tmp, &my_list) {
		entry = list_entry(i, struct pid_list, list);
		printk(KERN_INFO "list traverse, pid:%d\n", entry->pid);
	}	
	del_pid(pids[0]);
	del_pid(pids[1]);
	del_pid(pids[2]);
	del_pid(pids[4]);
	del_pid(pids[8]);
	
	
	printk(KERN_INFO "After del_pid()...\n");

	list_for_each_safe(i, tmp, &my_list) {
		entry = list_entry(i, struct pid_list, list);
		printk(KERN_INFO "list traverse, pid:%d\n", entry->pid);
	}	
	*/

	return 0; 
}

static void __exit list_module_exit(void) 
{
	printk(KERN_INFO "Module exiting ...\n");
	destroy_list();
}

module_init(list_module_init);
module_exit(list_module_exit);
MODULE_LICENSE("GPL"); 
