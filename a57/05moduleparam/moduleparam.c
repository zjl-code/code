#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE ("GPL");

int irq = 0;
char * str = "wednesday";
int fish[10] = {0};
int num = 10;

module_param (irq, int, 0644);
module_param (str, charp, 0);
module_param_array (fish, int, &num, 0600);

int __init moduleparam_init (void)
{
    int i = 0;
    printk (KERN_ALERT "irq = %d\n", irq);
    printk (KERN_ALERT "str = %s\n", str);
    for (; i<num; i++)
    {
        printk (KERN_ERR "fish[%d] = %d\n", i, fish[i]);
    }
    return 0;
}
void __exit moduleparam_exit (void)
{
    int i = 0;
    printk (KERN_ALERT "irq = %d\n", irq);
    printk (KERN_ALERT "str = %s\n", str);
    for (; i<num; i++)
    {
        printk (KERN_ERR "fish[%d] = %d\n", i, fish[i]);
    }

    return ;
}
module_init (moduleparam_init);
module_exit (moduleparam_exit);



