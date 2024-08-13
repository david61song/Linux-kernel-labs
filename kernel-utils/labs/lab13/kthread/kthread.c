/*
 * Part of Lab 13 - Deferred Work
 * Implement a simple module that creates a kernel thread 
 * showing the current identifier
 * Exercise #6: kernel thread
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <asm/atomic.h>

MODULE_DESCRIPTION("Simple kernel thread");
MODULE_AUTHOR("david61song <david61song@gmail.com>");
MODULE_LICENSE("GPL");

/* Some kind of blocked (sleeping) task list */
static wait_queue_head_t wq_stop_thread;
/* flag that check if should we stop this kernel thread which waits stop the task */
static atomic_t flag_stop_thread;
/* Some kind of blocked (sleeping) task list which waits thread terminated */
static wait_queue_head_t wq_thread_terminated;
/* flag that check if we finshed this kernel thread */
static atomic_t flag_thread_terminated;
/* represent kthread */
static struct task_struct *my_thread;

static int my_thread_f(void *data)
{
	pr_info("[my_thread_f] Current process id is %d (%s)\n",
		current->pid, current->comm);

	/* Wait for command to remove module on wq_stop_thread queue. */
	/* NOTE: this kernel thread (which runs in process context) goes to sleep in wait_queue
	 * until the condition evaluates to true (could be any expression in C)
	 * use atomic_t to prevent race condition
	 * NOTE : wake_up() has to be called after changing any variable that could
 	 * change the result of the wait condition.
	 */
	
	wait_event_interruptible(wq_stop_thread, atomic_read(&flag_stop_thread));
	printk(KERN_INFO "[my_thread_f] Kernel thread will be terminated!");

	/* After this code, kernel thread already have woken up */
	/* Set flag to mark that kernel thread termination compeleted */
	atomic_set(&flag_thread_terminated, 1);
	wake_up(&wq_thread_terminated);

	pr_info("[my_thread_f] Exiting\n");
	return 0; 
}

static int __init my_kthread_init(void)
{
	pr_info("[kthread_init] Init module\n");

	/* Initialize the waitqueues and flags */
	init_waitqueue_head(&wq_stop_thread);
	init_waitqueue_head(&wq_thread_terminated);
	atomic_set(&flag_stop_thread, 0);
	atomic_set(&flag_thread_terminated, 0);

	/* Create and start the kernel thread */
	my_thread = kthread_run(my_thread_f, NULL, "my_kthread");

	pr_info("[kthread_init] Kernel thread started\n");

	return 0;
}

static void __exit my_kthread_exit(void)
{
	pr_info("[kthread_exit] Exit module\n");

	/* Notify the kernel thread that it's time to exit */
	atomic_set(&flag_stop_thread, 1);
	wake_up(&wq_stop_thread);

	/* Wait for the kernel thread to exit */
	wait_event_interruptible(wq_thread_terminated, atomic_read(&flag_thread_terminated));

	/* We checked the kernel thread finshed. Unload the moudle */
	pr_info("[kthread_exit] Kernel thread terminated\n");
}

module_init(my_kthread_init);
module_exit(my_kthread_exit);
