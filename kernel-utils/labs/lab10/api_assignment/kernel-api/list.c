/*
 * list.c - Linux kernel list API
 * Author: Eunseok Song (david61song@gmail.com)
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define PROCFS_MAX_SIZE     1024
#define NAME_MAX_SIZE       128

#define procfs_dir_name     "list"
#define procfs_file_read    "preview"
#define procfs_file_write   "management"

struct proc_dir_entry *proc_list;
struct proc_dir_entry *proc_list_read;
struct proc_dir_entry *proc_list_write;

struct name_entry {
    char name[NAME_MAX_SIZE];
    struct list_head list;
};

/* Define list name to name_list */
static LIST_HEAD(name_list);

/* Begin of some list modify method */
static int add_to_list(char *name, char mode) {
    if (mode != 'f' && mode != 'e')
        return -1;
    if (strlen(name) >= NAME_MAX_SIZE)
        return -1;

    struct name_entry *entry = kmalloc(sizeof(struct name_entry), GFP_KERNEL);
    if (!entry)
        return -ENOMEM;

    /* We already checked size of name */
    strcpy(entry->name, name);
    INIT_LIST_HEAD(&entry->list);

    switch(mode) {
        case 'f':
            list_add(&entry->list, &name_list);
            break;
        case 'e':
            list_add_tail(&entry->list, &name_list);
            break;
    }

    return 0;
}
static int delete_first_from_list(char *name) {
    struct name_entry *curr_name_entry;
    struct name_entry *next;

    list_for_each_entry_safe(curr_name_entry, next, &name_list, list) {
        if (strcmp(curr_name_entry->name, name) == 0) {
            list_del(&curr_name_entry->list);
            kfree(curr_name_entry);
            return 0;
        }
    }

    return -1;
}
static int delete_all_from_list(char *name) {
    struct name_entry *curr_name_entry;
    struct name_entry *next;

    list_for_each_entry_safe(curr_name_entry, next, &name_list, list) {
        if (strcmp(curr_name_entry->name, name) == 0) {
            list_del(&curr_name_entry->list);
            kfree(curr_name_entry);
        }
    }
    return 0;
}

static int destroy_list(void) {
    struct name_entry *curr_name_entry;
    struct name_entry *next;

    list_for_each_entry_safe(curr_name_entry, next, &name_list, list) {
            list_del(&curr_name_entry->list);
            kfree(curr_name_entry);
    }
    return 0;
}
/* End of some list name method */


/* show list function */
static int list_proc_show(struct seq_file *m, void *v)
{
    struct name_entry *entry;
    list_for_each_entry(entry, &name_list, list) {
        seq_printf(m, "%s", entry->name);
    }

    return 0;
}

static int list_read_open(struct inode *inode, struct file *file)
{
    return single_open(file, list_proc_show, NULL);
}

static int list_write_open(struct inode *inode, struct file *file)
{
    return single_open(file, list_proc_show, NULL);
}

/* Write to /proc/list/management some command and name */

static ssize_t list_write(struct file *file, const char __user *buffer,
              size_t count, loff_t *offs)
{
    char local_buffer[PROCFS_MAX_SIZE];
    unsigned long local_buffer_size = 0;
    char inst[5];
    char name[NAME_MAX_SIZE + 1];
    unsigned int name_size;

    local_buffer_size = count;
    if (local_buffer_size > PROCFS_MAX_SIZE)
        local_buffer_size = PROCFS_MAX_SIZE;

    memset(local_buffer, 0, PROCFS_MAX_SIZE);

    /* safely copy data from userspace */
    if (copy_from_user(local_buffer, buffer, local_buffer_size))
        return -EFAULT;

    /* local_buffer contains your command written in /proc/list/management */
    /* parse inst and name */
    strncpy(inst, local_buffer, 4);
    inst[4] = '\0';

    name_size = local_buffer_size - 5;

    /* if name size exceeds NAME_MAX_SIZE, raises an error. */
    if (name_size >= NAME_MAX_SIZE)
        return -EINVAL;

    /* We had already checked name size, just copy it. */
    strncpy(name, local_buffer + 5, name_size);
    name[name_size] = '\0';


    /* compare instruction and call function */
    if (strcmp(inst, "addf") == 0) {
        add_to_list(name, 'f');
    } else if (strcmp(inst, "adde") == 0) {
        add_to_list(name, 'e');
    } else if (strcmp(inst, "delf") == 0) {
        delete_first_from_list(name);
    } else if (strcmp(inst, "dela") == 0) {
        delete_all_from_list(name);
    } else {
        return -EINVAL;
    }

    return local_buffer_size;
}

static const struct proc_ops r_pops = {
    .proc_open       = list_read_open,
    .proc_read       = seq_read,
    .proc_lseek      = seq_lseek,
    .proc_release    = single_release,
};

static const struct proc_ops w_pops = {
    .proc_open       = list_write_open,
    .proc_write      = list_write,
    .proc_release    = single_release,
};

/* Initialization of /proc entry */

static int __init list_init(void)
{
    proc_list = proc_mkdir(procfs_dir_name, NULL); //making "list" dir to /proc
    if (!proc_list)
        return -ENOMEM;

    proc_list_read = proc_create(procfs_file_read, 0444, proc_list,
                     &r_pops);
    if (!proc_list_read)
        goto proc_list_cleanup;

    proc_list_write = proc_create(procfs_file_write, 0222, proc_list,
                      &w_pops);
    if (!proc_list_write)
        goto proc_list_read_cleanup;

    return 0;

/* Error Handling */

proc_list_read_cleanup:
    proc_remove(proc_list_read);
proc_list_cleanup:
    proc_remove(proc_list);
    return -ENOMEM;
}

static void __exit list_exit(void)
{
    destroy_list();
    proc_remove(proc_list_write);
    proc_remove(proc_list_read);
    proc_remove(proc_list);
}

module_init(list_init);
module_exit(list_exit);

MODULE_DESCRIPTION("Linux kernel list API");
MODULE_AUTHOR("Eunseok Song david61song@gmail.com");
MODULE_LICENSE("GPL v2");