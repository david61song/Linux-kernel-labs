/*
 * Lab 13 - Deferred Work
 *
 * Exercises #3, #4, #5: deferred work
 *
 * Modified by Eunseok Song
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sched/task.h>
#include "../include/deferred.h"
#include "linux/jiffies.h"
#include "linux/libata.h"
#include "linux/spinlock.h"
#include "linux/types.h"

#define MY_MAJOR		42
#define MY_MINOR		0
#define MODULE_NAME		"deferred"

#define TIMER_TYPE_NONE		-1
#define TIMER_TYPE_SET		0
#define TIMER_TYPE_ALLOC	1
#define TIMER_TYPE_MON		2

MODULE_DESCRIPTION("Deferred work character device");
MODULE_AUTHOR("david61song <david61song@gmail.com>");
MODULE_LICENSE("GPL");


struct mon_proc {
	struct task_struct *task;
	struct list_head list;
};

static struct my_device_data {
	struct cdev cdev;
	struct timer_list timer; 			/* timer */
	int flag;							/* flag of timer type */
	struct work_struct io_work;			/* io_alloc work */
	struct work_struct monitor_work;	/* mointor work */
	struct list_head process_list;		/* list for monitored processes */
	spinlock_t lock; 					/* spinlock to protect list */
} dev;


/* This function simulate long time processing. Don't process it in
 * interrupt context, process it workqueue.
 */
static void alloc_io(void)
{
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(5 * HZ);
	pr_info("Yawn! I've been sleeping for 5 seconds.\n");
}

/* This function malloc'ing to kernel memory space. 
 * Need to free'ing after done to use!
 */
static struct mon_proc *get_proc(pid_t pid)
{
	struct task_struct *task;
	struct mon_proc *p;

	rcu_read_lock();
	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	rcu_read_unlock();
	if (!task)
		return ERR_PTR(-ESRCH);

	p = kmalloc(sizeof(*p), GFP_ATOMIC);
	if (!p)
		return ERR_PTR(-ENOMEM);

	get_task_struct(task);
	p->task = task;

	return p;
}


/* define work handler for alloc_io() */
static void io_work_handler (struct work_struct *work){
	alloc_io();
}

static void monitor_work_handler (struct work_struct *work) {
    struct mon_proc *proc_ptr, *tmp;

    spin_lock(&dev.lock);
    list_for_each_entry_safe(proc_ptr, tmp, &dev.process_list, list) {
        if (proc_ptr->task->__state == TASK_DEAD || proc_ptr->task->exit_state) {
            pr_info("===== task %s(%d) is dead!!! ===== \n", proc_ptr->task->comm, proc_ptr->task->pid);
            /* Decrement the task usage counter */
            put_task_struct(proc_ptr->task);
            /* Remove the process from the list */
            list_del(&proc_ptr->list);
            /* Free the memory allocated for the mon_proc struct */
            kfree(proc_ptr);
        }
    }
    spin_unlock(&dev.lock);
}

static void timer_handler(struct timer_list *tl)
{
    printk(KERN_INFO "[timer_handler] pid = %d, comm = %s", current->pid, current->comm);

	if (dev.flag == TIMER_TYPE_SET) {
		mod_timer(&dev.timer, jiffies + (1) * HZ);
	}
	else if (dev.flag == TIMER_TYPE_ALLOC){
		schedule_work(&dev.io_work);
	}
	else if (dev.flag == TIMER_TYPE_MON){
		schedule_work(&dev.monitor_work);
		mod_timer(&dev.timer, jiffies + (1) * HZ);
	}

}

static int deferred_open(struct inode *inode, struct file *file)
{
	struct my_device_data *my_data =
		container_of(inode->i_cdev, struct my_device_data, cdev);
	file->private_data = my_data;
	pr_info("[deferred_open] Device opened\n");
	return 0;
}

static int deferred_release(struct inode *inode, struct file *file)
{
	pr_info("[deferred_release] Device released\n");
	return 0;
}

static long deferred_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct my_device_data *my_data = (struct my_device_data*) file->private_data;
	struct mon_proc *proc_ptr;

	pr_info("[deferred_ioctl] Command: %s\n", ioctl_command_to_string(cmd));

	switch (cmd) {
		case MY_IOCTL_TIMER_SET:
			/* set timer to run after <seconds> seconds */
			/* set flag and schedule timer */
			my_data->flag = TIMER_TYPE_SET;
			mod_timer(&my_data->timer, jiffies + (arg) * HZ);
			break;
		case MY_IOCTL_TIMER_CANCEL:
			/* cancel timer (delete timer)*/
			del_timer(&my_data->timer);
			break;
		/* allocate memory after <seconds> seconds */
		case MY_IOCTL_TIMER_ALLOC: 
			/* invoke alloc_io() */
			/* set flag and schedule timer */
			my_data->flag = TIMER_TYPE_ALLOC;
			mod_timer(&my_data->timer, jiffies + arg * HZ);
			break;
		/* monitor pid */
		case MY_IOCTL_TIMER_MON: //monitor
		{
			/* use get_proc() and add task to list. PROCTECT THE LIST! */
			proc_ptr = get_proc(arg);
			if (proc_ptr == NULL)
				return -ENOTTY;

			spin_lock(&my_data->lock);
			list_add(&proc_ptr->list, &my_data->process_list);
			spin_unlock(&my_data->lock);

			/* set flag and scheduler timer */
			my_data->flag = TIMER_TYPE_MON;
			mod_timer(&my_data->timer, jiffies + (1) * HZ);
			break;
		}
		default:
			return -ENOTTY;
	}
	return 0;
}

struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = deferred_open,
	.release = deferred_release,
	.unlocked_ioctl = deferred_ioctl,
};

static int deferred_init(void)
{
	int err;

	pr_info("[deferred_init] Init module\n");
	err = register_chrdev_region(MKDEV(MY_MAJOR, MY_MINOR), 1, MODULE_NAME);
	if (err) {
		pr_info("[deffered_init] register_chrdev_region: %d\n", err);
		return err;
	}

	/* Initialize flag. */
	dev.flag = TIMER_TYPE_NONE;
	/* Initialize work. */

	INIT_WORK(&dev.io_work, io_work_handler);
	INIT_WORK(&dev.monitor_work, monitor_work_handler);

	/* Initialize lock and list. */
	spin_lock_init(&dev.lock);
	INIT_LIST_HEAD(&dev.process_list);

	cdev_init(&dev.cdev, &my_fops);
	cdev_add(&dev.cdev, MKDEV(MY_MAJOR, MY_MINOR), 1);

	/* Initialize timer. */
	// Acknowledge the kernel that we will use this timer
	timer_setup(&dev.timer, timer_handler, 0);

	return 0;
}

static void deferred_exit(void)
{
    struct mon_proc *p, *n;

    pr_info("[deferred_exit] Exit module\n");
	// delete char device 
    cdev_del(&dev.cdev);
    unregister_chrdev_region(MKDEV(MY_MAJOR, MY_MINOR), 1);
    // Cleanup make sure the timer is not running after exiting. 
    del_timer_sync(&dev.timer);
    // Cleanup: make sure the work handler is not scheduled.
    flush_work(&dev.io_work);
    flush_work(&dev.monitor_work);
    // Cleanup the monitored process list
    spin_lock(&dev.lock);
    list_for_each_entry_safe(p, n, &dev.process_list, list) {
        // decrement task usage counter
        put_task_struct(p->task);
        // remove it from the list
        list_del(&p->list);
        // free the struct mon_proc 
        kfree(p);
    }
    spin_unlock(&dev.lock);
}

module_init(deferred_init);
module_exit(deferred_exit);
