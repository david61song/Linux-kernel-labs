#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/random.h>

static int res = 0;
static int n;
module_param(n, int, 0);

static int get_nearest_sqrt(int num) {
    int iter = 1;
    while (iter * iter < num) {
        iter++;
    }
    return iter;
}

static int check_if_prime(int num) {
    if (num < 0)
        num *= -1;
    if (num < 2) 
        return 0; 
    int sqrt_of_num = get_nearest_sqrt(num);
    for (int iter = 2; iter <= sqrt_of_num; iter++) {
        if (num % iter == 0)
            return 0;
    }
    return 1;
}

static long count_rand_prime(int size) {
    int *kbuffer;
    ktime_t start, end;
    long duration;

    kbuffer = kmalloc(sizeof(int) * size, GFP_KERNEL);
    if (!kbuffer) {
        printk(KERN_ALERT "Failed to allocate memory\n");
        return -1;
    }

    start = ktime_get();
    get_random_bytes(kbuffer, sizeof(int) * size);

    /* Convert to integers and ensure they don't exceed max */
    for (int i = 0; i < size; i++) {
        kbuffer[i] = (unsigned int)kbuffer[i] % (size + 1);
    }

    for (int i = 0; i < size; i++) {
        if (check_if_prime(kbuffer[i]))
            res++;
    }
    end = ktime_get();
    kfree(kbuffer);
    duration = ktime_to_ns(ktime_sub(end, start)) / 1000; // Convert to microseconds
    return duration;
}

static int __init lkp_init(void)
{
    long interval;
    printk(KERN_ALERT "The Number of total random numbers %d\n", n);
    interval = count_rand_prime(n);
    printk(KERN_ALERT "The Number of prime numbers %d\n", res);
    printk(KERN_ALERT "Processing time : %ld us\n", interval);
    return 0;
}

static void __exit lkp_exit(void)
{
    printk(KERN_ALERT "Module de-loaded\n");
}

module_init(lkp_init);
module_exit(lkp_exit);
MODULE_LICENSE("GPL");
