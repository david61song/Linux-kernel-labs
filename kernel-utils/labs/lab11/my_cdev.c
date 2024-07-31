/*
 * Character device drivers lab
 *
 * All tasks
 *
 * Skeleton developed by UPB, modified for Ajou Univ. SCE394.
 * Compeleted by Eunseok Song, (@david61song)
 *
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <asm/ioctl.h>

#include "my_cdev.h"

MODULE_DESCRIPTION("Lab 11 character device implementation");
MODULE_AUTHOR("Eunseok Song");
MODULE_LICENSE("GPL");


/* Buffer size */
#ifndef BUFSIZ
#define BUFSIZ		4096
#endif

/* Device identification number */
#define MY_MAJOR 42
#define MY_MAX_MINORS 5
#define FIRST_MINOR_NUM 0



struct my_device_data {
	struct cdev cdev; /* cdev structure data */
	char device_buffer[BUFSIZ]; /* our device buffer */
	atomic_t access; /* atomic_t access variable size to track if file is opened */
	size_t size; /* buffer size */
};

/* array of device data */
/* we define my_device_data array that holds device data which have size FIRST_MINOR_NUM 
 * That is, we assume that we have MY_MAX_MINORS number of devices. */

struct my_device_data devs[MY_MAX_MINORS]; 


static int my_open(struct inode *inode, struct file *file)
{
	struct my_device_data *my_data;
	/* print message when the device file is open. */

	/* inode->i_cdev contains our cdev struct, use container_of to obtain a pointer to my_device_data */
	/* The purpose of the container_of macro is to find the starting address of a structure
	 * when the address of one of its members is known.
	 */

	/* Hints: container_of(ptr, type, member) */
	my_data = container_of(inode->i_cdev, struct my_device_data, cdev);
	file->private_data = my_data;

	/* return immediately if access is != 0, use atomic_cmpxchg */
	if(atomic_cmpxchg(&my_data->access, 0, 1) != 0)
		return -EBUSY;
	printk("Device opened");
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

	atomic_set(&my_data->access, 0);
	printk("Device closed (released)");

	return 0;
}

static ssize_t my_read(struct file *file,
		char __user *user_buffer,
		size_t size, loff_t *offset)
{
	struct my_device_data *my_data = (struct my_device_data *) file->private_data;
	ssize_t len = min(my_data->size - *offset, size);

	if (len <= 0)
	    return 0;

	if (copy_to_user(user_buffer, my_data->device_buffer + *offset, len))
		return -EFAULT;

	*offset += len;

	return len;
}

static ssize_t my_write(struct file *file,
		const char __user *user_buffer,
		size_t size, loff_t *offset)
{
    struct my_device_data *my_data = (struct my_device_data *) file->private_data;
	/*  my_data->size - *offset ==> remained buffer size
	 *	size -> size to trying to write 
	 */
    size_t len = min(my_data->size - *offset, size);
	/* we cannot write more data to our kernelspace buffer (device buffer)*/
    if (len <= 0){ 
		return 0;
	}

    if (copy_from_user(my_data->device_buffer + *offset, user_buffer, len)){
		return -EFAULT;
	}

	/* update our offset */
    *offset += len;
    return len;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct my_device_data *my_data = (struct my_device_data *) file->private_data;
	int ret = 0;
	int remains;

	switch (cmd) {
	    /* if cmd = MY_IOCTL_PRINT, display IOCTL_MESSAGE */
	    case MY_IOCTL_PRINT:
			printk(KERN_INFO "%s\n", IOCTL_MESSAGE);
			break;
	    /* if cmd = MY_IOCTL_GET_BUFFER, read my_data->buffer to arg */
	    case MY_IOCTL_GET_BUFFER:
			if (copy_to_user((char __user *)arg, my_data->device_buffer, my_data->size))
		    	ret = -EFAULT;
			break;
	    /* if cmd = MY_IOCTL_SET_BUFFER, write arg to my_data->buffer */
	    case MY_IOCTL_SET_BUFFER:
			remains = copy_from_user(my_data->device_buffer, (char __user *)arg, BUFFER_SIZE);
			if (remains) //if copy_from_user fails
		    	ret = -EFAULT;
			my_data->size = BUFFER_SIZE - remains;
			break;
	    /* Unknown command */
		default:
			ret = -EINVAL;
	}

	return ret;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
	.unlocked_ioctl = my_ioctl
};

static int __init my_init_module(void)
{
	int err;
	int i;

	/* register char device region for MY_MAJOR and MY_MAX_MINORS starting at 0 */
	/* reserves it */
	err = register_chrdev_region(MKDEV(MY_MAJOR, FIRST_MINOR_NUM), MY_MAX_MINORS, "mydev");

	if (err != 0){
	    return err;
	}

	printk(KERN_INFO "Init device module...");

	for (i = 0; i < MY_MAX_MINORS; i++) {
		/* character device will have to be initialized */
		cdev_init(&devs[i].cdev, &my_fops);
		/* kernel will have to be notified */
		cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, i), 1); 
		/* atomic varaible set */
		atomic_set(&devs[i].access, 0);
		/* size of transfer data size */
		devs[i].size = BUFSIZ;

	}

	return 0;
}

static void __exit my_cleanup_module(void)
{
    int i;
    /* delete cdev from kernel core */
    for (i = 0 ; i < MY_MAX_MINORS ; i++){
    /* unregister char device region, for MY_MAJOR and MY_MAX_MINORS starting at 0 */
		cdev_del(&devs[i].cdev);
    }
	printk(KERN_INFO "Cleanup device module...");
    unregister_chrdev_region(MKDEV(MY_MAJOR,0), MY_MAX_MINORS);
}

module_init(my_init_module);
module_exit(my_cleanup_module);
