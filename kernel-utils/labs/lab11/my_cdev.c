/*
 * Character device drivers lab
 *
 * All tasks
 *
 * Skeleton developed by UPB, modified for Ajou Univ. SCE394.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
/* TODO: add header files to complete thie module */

#include "my_cdev.h"

MODULE_DESCRIPTION("SCE394 character device");
MODULE_AUTHOR("SCE394");
MODULE_LICENSE("GPL");

#ifndef BUFSIZ
#define BUFSIZ		4096
#endif


struct my_device_data {
	/* TODO: add cdev member */
	/* TODO: add buffer with BUFSIZ elements */
	/* TODO: add read/write buffer size */
	/* TODO: add atomic_t access variable to keep track if file is opened */
	atomic_t access;
};

static size_t my_min(size_t n1, size_t n2) {
	if (n1 <= n2)
		return n1;
	else
		return n2;
}

static int my_open(struct inode *inode, struct file *file)
{

	/* TODO: print message when the device file is open. */

	/* TODO: inode->i_cdev contains our cdev struct, use container_of to obtain a pointer to my_device_data */

	/* TODO: return immediately if access is != 0, use atomic_cmpxchg */
	if(atomic_cmpxchg(&my_data->access, 0, 1) != 0)
		return -EBUSY;

	/*
	 *  If you want to test open after relase device file, 
	 *  use below functions to sleep open().
	 */
	// set_current_state(TASK_INTERRUPTIBLE);
	// schedule_timeout(10 * HZ);

	return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
	struct my_device_data *my_data =
		(struct my_device_data *) file->private_data;
	/* TODO: print message when the device file is closed. */

	/* TODO: reset acces varable to 0, use atomic_set */
	atomic_set(&my_data->access, 0);

	return 0;
}

static ssize_t my_read(struct file *file,
		char __user *user_buffer,
		size_t size, loff_t *offset)
{
	size_t len;

	/* TODO: Copy my_data->buffer to user_buffer, use copy_to_user */

	return len;
}

static ssize_t my_write(struct file *file,
		const char __user *user_buffer,
		size_t size, loff_t *offset)
{
	/* TODO: copy user_buffer to my_data->buffer, use copy_from_user */

	return size;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct my_device_data *my_data = (struct my_device_data *) file->private_data;
	int ret = 0;
	int remains;

	switch (cmd) {
		/* TODO: if cmd = MY_IOCTL_PRINT, display IOCTL_MESSAGE */
		/* TODO: if cmd = MY_IOCTL_SET_BUFFER, write arg to my_data->buffer */
		/* TODO: if cmd = MY_IOCTL_GET_BUFFER, read my_data->buffer to arg */
		default:
			ret = -EINVAL;
	}

	return ret;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	/* TODO: add open and release functions */
	/* TODO: add read function */
	/* TODO: add write function */
	/* TODO: add ioctl function */
};

static int __init my_init_module(void)
{
	int err;
	int i;

	/* TODO: register char device region for MY_MAJOR and MY_MAX_MINORS starting at 0 */

	for (i = 0; i < MY_MAX_MINORS; i++) {
		/* TODO: init and add cdev to kernel core */
		/* TODO: set access variable to 0, use atomic_set */
		atomic_set(&devs[i].access, 0);
		/* TODO: initialize buffer with MESSAGE string */
	}

	return 0;
}

static void __exit my_cleanup_module(void)
{
	/* TODO: delete cdev from kernel core */
	/* TODO: unregister char device region, for MY_MAJOR and MY_MAX_MINORS starting at 0 */
}

module_init(my_init_module);
module_exit(my_cleanup_module);
