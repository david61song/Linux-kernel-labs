/*
 * Deferred Work
 * Exercise #1, #2: simple timer
 * Modified by Eunseok Song
 * Simple kernel timer prints message every TIMER_TIMEOUT
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

MODULE_DESCRIPTION("Simple kernel timer");
MODULE_AUTHOR("david61song <david61song@gmail.com>");
MODULE_LICENSE("GPL");

#define TIMER_TIMEOUT 1

static struct timer_list timer;

static void timer_handler(struct timer_list *tl)
{
    /* Check if we are in interrupt context */
    if (in_interrupt()) {
        printk(KERN_INFO "Timer handler is running in interrupt context\n");
    } else {
        printk(KERN_INFO "Timer handler is running in process context\n");
    }

    /* print a message */
    printk(KERN_INFO "%d seconds elapsed...!", TIMER_TIMEOUT);

    /* reschedule timer */
    mod_timer(&timer, jiffies + TIMER_TIMEOUT * HZ);
}

static int __init timer_init(void)
{
	pr_info("[timer_init] Init module\n");

	/* initialize timer */
	timer_setup(&timer, timer_handler, 0);
	/* schedule timer for the first time */
	mod_timer(&timer, jiffies + TIMER_TIMEOUT * HZ);

	return 0;
}

static void __exit timer_exit(void)
{
	pr_info("[timer_exit] Exit module\n");

	/* Must stop timer */
	del_timer(&timer);
}

module_init(timer_init);
module_exit(timer_exit);
