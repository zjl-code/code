#include <linux/init.h>
#include <linux/module.h>
#include "export.h"
MODULE_LICENSE ("GPL");

int __init import_init (void)
{
    int res = add (10, 20);
    printk (KERN_ALERT "res = %d\n", res );
    return 0;
}
void __exit import_exit (void)
{
    printk (KERN_ALERT "bye bye!\n");
    return ;
}
module_init (import_init);
module_exit (import_exit);
