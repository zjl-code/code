#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE ("GPL");

dev_t dev;//保存设备号
unsigned int major = 0;
unsigned int minor = 10;
int __init led_drv_init (void)
{
    if (major)
    {
        dev = (major << 20) | minor;
        //dev = MKDEV (major, minor);和上面一样
        register_chrdev_region (dev, 1, "leds");
    }
    else
    {
        alloc_chrdev_region (&dev, 100, 1, "leds");
        major = MAJOR (dev);
        minor = MINOR (dev);
        printk (KERN_ERR "major = %d\n,minor = %d\n",major, minor);
    }
        return 0;
}
void __exit led_drv_exit (void)
{
    unregister_chrdev_region (dev, 1);
    return ;
}
module_init (led_drv_init);
module_exit (led_drv_exit);
