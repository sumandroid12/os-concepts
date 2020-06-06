/**
 * task_info.c
 *
 * Kernel module that communicates with /proc file system.
 * 
 * The makefile must be modified to compile this program.
 * Change the line "simple.o" to "hello.o"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 128

#define PROC_NAME "taskinfo"
#define MESSAGE "Hello World\n"

/**
 * Function prototypes
 */
static ssize_t my_proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static ssize_t my_proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops1 = {
    .proc_read = my_proc_read,
    .proc_write = my_proc_write};
long pid;

/* This function is called when the module is loaded. */
static int proc_init(void)
{

        // creates the /proc/hello entry
        // the following function call is a wrapper for
        // proc_create_data() passing NULL as the last argument
        proc_create(PROC_NAME, 0, NULL, &proc_ops1);

        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

        return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)
{

        // removes the /proc/hello entry
        remove_proc_entry(PROC_NAME, NULL);

        printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/hello is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */
static ssize_t my_proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        struct task_struct *p_task;
        struct pid *vpid;
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;

        /* NOIDEA */
        if (completed)
        {
                completed = 0;
                return 0;
        }

        completed = 1;
        /* NOIDEA */

        vpid = find_vpid(pid);
        p_task = pid_task(vpid, PIDTYPE_PID);

        if (p_task)
                rv = sprintf(buffer, "command = [%s] pid = [%d] state = [%ld]\n", p_task->comm, p_task->pid, p_task->state);
        else
                rv = sprintf(buffer, "invalid pid\n");

        // copies the contents of buffer to userspace usr_buf
        copy_to_user(usr_buf, buffer, rv);

        return rv;
}

static ssize_t my_proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
        char *k_mem;
        /* allocate kernel memory */
        k_mem = kmalloc(count, GFP_KERNEL);
        /* copies user space usr buf to kernel memory */
        copy_from_user(k_mem, usr_buf, count);
        printk(KERN_INFO "%s∖n", k_mem);
        kstrtol(k_mem, 10, &pid);
        /* return kernel memory */
        kfree(k_mem);
        return count;
}

/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OS practice: Task Info Module");
MODULE_AUTHOR("Suman Saha");