/**
 * task_tree.c
 *
 * Kernel module that communicates with /proc file system.
 * 
 * The makefile must be modified to compile this program.
 * Change the line "simple.o" to "task_tree.o"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/list.h>

#define BUFFER_SIZE 128

#define MOD_NAME "tasktree"
#define MESSAGE "Hello World\n"

/**
 * Function prototypes
 */

long pid;
struct task_struct *task;

void list_proc_recursively(struct task_struct *p_task);

int mod_init(void)
{
    printk(KERN_INFO "Initializing task tree module\n");
    for_each_process(task)
    {
        printk(KERN_INFO MOD_NAME " :command=[%s] pid=[%d] state=[%ld]\n", task->comm, task->pid, task->state);
        list_proc_recursively(task);
    }
    printk(KERN_INFO MOD_NAME " :finished listing processes\n");
    return 0;
}

// recursive dfs
void list_proc_recursively(struct task_struct *p_task)
{
    /* declare local variables for recursive calls 
    previously made the mistake of declaring these globally */
    struct task_struct *c_task;
    struct list_head *list;
    list_for_each(list, &p_task->children)
    {
        c_task = list_entry(list, struct task_struct, sibling);
        printk(KERN_INFO MOD_NAME " :command=[%s] pid=[%d] state=[%ld]\n", c_task->comm, c_task->pid, c_task->state);
        list_proc_recursively(c_task);
    }
}

void mod_exit(void)
{
    printk(KERN_INFO "MOD_NAME :Removing kernel module\n");
}
/* Macros for registering module entry and exit points. */
module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OS practice: Task Info Module");
MODULE_AUTHOR("Suman Saha");
