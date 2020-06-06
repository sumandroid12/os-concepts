#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/list.h>

#define MOD_NAME "collatz"

static int start = 25;
module_param(start, int, 0);

void collatz(int input)
{
    printk("%d ", input);
    while (input != 1)
    {
        if (input % 2 == 0)
        {
            input /= 2;
        }
        else
        {
            input = input * 3 + 1;
        }
        printk("%d ", input);
    }
}

int mod_init(void)
{
    printk(KERN_INFO "Initializing " MOD_NAME " module\n");
    collatz(start);
    return 0;
}

void mod_exit(void)
{
    printk(KERN_INFO "Exiting " MOD_NAME " module\n");
}
/* Macros for registering module entry and exit points. */
module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OS practice: Task Info Module");
MODULE_AUTHOR("Suman Saha");