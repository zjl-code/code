#include <linux/init.h>
#include <linux/module.h>

int __init hello_world_init (void)
{
	printk (KERN_ALERT "hello world!\n");
	return 0;
}
void __exit hello_world_exit (void)
{

	printk (KERN_ALERT "bye!\n");
	return ;
}
module_init (hello_world_init);
module_exit (hello_world_exit);

