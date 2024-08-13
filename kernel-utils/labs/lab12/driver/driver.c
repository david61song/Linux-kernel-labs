/***************************************************************************//**
 *  \file		driver.c
 *
 *  \details		Interrupt Example
 *
 *  \author		EmbeTronicX
 *
 *  \modified by	Eunseok Song
 *
 *******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
// #include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
// #include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
// #include<linux/sysfs.h> 
// #include<linux/kobject.h> 
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/hw_irq.h>

#include <linux/spinlock.h>

#include "driver.h"
#include "linux/mutex_types.h"

#define MY_IRQ_NO		11

#define MY_MAJOR	42
#define MY_MAX_MINORS	1

#define DEVICE		"mycdev"


/* use this lock or more lock variables if you want */
static spinlock_t lock;
static DEFINE_MUTEX(dev_mutex);
// To raise IRQ, you must modify kernel codes
// Add the EXPORT_SYMBOL_GPL(vector_irq) after #include definition in arch/x86/kernel/irq.c
// Add the EXPORT_SYMBOL_GPL(irq_to_desc) after irq_to_desc() in kernel/irq/irqdesc.c
// After then, we need to rebuild the linux source: make -j$(nproc)


struct my_device_data {
	struct cdev cdev;
	int num[5];
	int sum;
};

struct my_device_data dev_data;

static int __init my_driver_init(void);
static void __exit my_driver_exit(void);

/*************** Driver Fuctions **********************/
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, 
		char __user *buf, size_t len,loff_t * off);
static ssize_t my_write(struct file *file, 
		const char *buf, size_t len, loff_t * off);
static ssize_t my_ioctl(struct file *file, 
		unsigned int cmd, unsigned long arg);
/************ End of Driver Fuctions *******************/


static struct file_operations fops =
{
	.owner          = THIS_MODULE,
	.read           = my_read,
	.write          = my_write,
	.open           = my_open,
	.release        = my_release,
	.unlocked_ioctl = my_ioctl
};

/* Interrupt handler for IRQ 11 
 * This Procedure runs with interrupt context, use spinlock 
 */
static irqreturn_t my_irq_handler(int irq, void *dev_id) {
	int i = 0;

	/* For preventing race condition by multiple runner, use spinlock */
	/* Do not use blocking methods lock! */

	spin_lock(&lock);
	dev_data.num[0] = -1;
	dev_data.num[1] = -2;
	dev_data.num[2] = -3;
	dev_data.num[3] = -4;
	dev_data.num[4] = -5;
	for (i = 0; i < 5; i ++)
		dev_data.sum += dev_data.num[i];
	spin_unlock(&lock);

	printk(KERN_INFO "IRQ, sum: %d\n", dev_data.sum);

	return IRQ_HANDLED;
}

static int my_open(struct inode *inode, struct file *file)
{
	struct my_device_data *my_data;
	my_data = container_of(inode->i_cdev, struct my_device_data, cdev);
	file->private_data = my_data;
	printk(KERN_INFO "Device File Opened...!!!\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Device File Closed...!!!\n");
	return 0;
}

static ssize_t my_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	struct my_device_data *my_data = (struct my_device_data *) file->private_data;
	int ret = 0;
	int sum = 0;
	int i;


	/* if you need spinlock or mutex, use them */
	switch (cmd) {
		case MY_IOCTL_PRINT0:
			printk(KERN_INFO "IOCTL0, sum: %d\n", my_data->sum);
			break;
		case MY_IOCTL_PRINT1:
			printk(KERN_INFO "IOCTL1, sum: %d\n", my_data->sum);
			break;
		case MY_IOCTL_PRINT2:
			printk(KERN_INFO "IOCTL2, sum: %d\n", my_data->sum);
			break;
		case MY_IOCTL_INC_0:
			my_data->num[0]++;
			break;
		case MY_IOCTL_INC_1:
			my_data->num[1]++;
			break;
		case MY_IOCTL_INC_2:
			my_data->num[2]++;
			break;
		case MY_IOCTL_INC_3:
			my_data->num[3]++;
			break;
		case MY_IOCTL_INC_4:
			my_data->num[4]++;
			break;
		case MY_IOCTL_INC_ALL:
			my_data->num[0]++;
			my_data->num[1]++;
			my_data->num[2]++;
			my_data->num[3]++;
			my_data->num[4]++;
			break;
		case MY_IOCTL_INT:
			asm("int $0x3B");  // Corresponding to irq 11
			break;
		case MY_IOCTL_SUM:
			for (i = 0; i < 5; i++)
				sum += my_data->num[i];
			my_data->sum = sum;
			break;
		case MY_IOCTL_RESET:
			my_data->num[0] = 0;
			my_data->num[1] = 0;
			my_data->num[2] = 0;
			my_data->num[3] = 0;
			my_data->num[4] = 0;
			my_data->sum = 0;
			break;
		case MY_IOCTL_LOCK:
			mutex_lock(&dev_mutex);
			break;
		case MY_IOCTL_UNLOCK:
			mutex_unlock(&dev_mutex);
			break;
		default:
			ret = -EINVAL;
	}
	return ret;
}

static ssize_t my_read(struct file *file, 
		char __user *buf, size_t size, loff_t *off)
{
	printk(KERN_INFO "Read function\n");

	return size;
}

static ssize_t my_write(struct file *file, 
		const char __user *buf, size_t size, loff_t *off)
{
	printk(KERN_INFO "Write Function\n");
	return size;
}

static int __init my_driver_init(void)
{
	int i;
	int err;
	unsigned long flags;
	struct irq_desc *desc;

	printk(KERN_INFO "Inserting module..!");
	/* Registering characther device to the system */
	if (register_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS, DEVICE)) {
		printk(KERN_INFO "Cannot register the device to the system\n");
		return -ENODEV;
	}

	/* Creating cdev structure*/
	cdev_init(&dev_data.cdev, &fops);

	/*Adding character device to the system*/
	if ((cdev_add(&dev_data.cdev, MKDEV(MY_MAJOR, 0), 1)) < 0){
		printk(KERN_INFO "Cannot add the device to the system\n");
		return -EBUSY;
	}

	/* initialize attributes of struct my_device_data */
	for (i = 0; i < 5; i ++)
		dev_data.num[i] = 0;
	dev_data.sum= 0;


	/* need to register irq number, handler */

	err = request_irq(MY_IRQ_NO, my_irq_handler, IRQF_SHARED, "my_irq_handler", &dev_data);
	/* init lock */
	spin_lock_init(&lock);

	/* Handling error */
	if (err < 0){
		printk(KERN_INFO "IRQ request failed!\n");
	    return err;
	}

	desc = irq_to_desc(MY_IRQ_NO);
	if (!desc) {
		return -EINVAL;
	}

	spin_lock_irqsave(&lock, flags);
	__this_cpu_write(vector_irq[59], desc);
	spin_unlock_irqrestore(&lock, flags);

	printk(KERN_INFO "Device Driver Insert...Done!!!\n");
	return 0;
}

static void __exit my_driver_exit(void)
{
	/* need to free irq, handler */
	free_irq(MY_IRQ_NO, NULL);
	cdev_del(&dev_data.cdev);
	unregister_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS);
	printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eunseok Song <david61song@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - for practicing Interrupts");
MODULE_VERSION("1.0");
