#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE ("GPL");

int __init printk_init (void)
{
    printk (KERN_EMERG "level 0\n");
    printk (KERN_ALERT "level 1\n");
    printk (KERN_CRIT "level 2\n");
    printk (KERN_ERR "level 3\n");
    printk (KERN_WARNING "level 4\n");
    printk (KERN_NOTICE "level 5\n");
    printk (KERN_INFO "level 6\n");
    printk (KERN_DEBUG "level 7\n");
    return 0;
}
void __exit printk_exit (void)
{
    printk (KERN_EMERG "level 0\n");
    printk (KERN_ALERT "level 1\n");
    printk (KERN_CRIT "level 2\n");
    printk (KERN_ERR "level 3\n");
    printk (KERN_WARNING "level 4\n");
    printk (KERN_NOTICE "level 5\n");
    printk (KERN_INFO "level 6\n");
    printk (KERN_DEBUG "level 7\n");

    return ;
}
module_init (printk_init);
module_exit (printk_exit);
