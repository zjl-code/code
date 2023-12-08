#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE ("GPL");
//定义一个cdev对象
struct cdev led_cdev;

dev_t dev;//保存设备号
unsigned int major = 0;//主设备号
unsigned int minor = 10;//次设备号
int led_open (struct inode *inode, struct file *filp )
{
    printk (KERN_ERR "enter:%s\n", __func__);
    return 0;
}
int led_close (struct inode *inode, struct file *filp )
{
    printk (KERN_ERR "enter:%s\n", __func__);
    return 0;
}
struct file_operations led_fops = //定义了file_operations结构体类型的对象led_fops
{
    .owner = THIS_MODULE,
    .open = led_open,
    .release = led_close, 
};
int __init led_drv_init (void)
{
    if (major)
    {
        //静态注册
        dev = (major << 20) | minor;
        //dev = MKDEV (major, minor);和上面一样
        register_chrdev_region (dev, 1, "leds");
    }
    else
    {   
        //动态注册，分配一个为被使用的设备号
        alloc_chrdev_region (&dev, 100, 1, "leds");
        major = MAJOR (dev);
        minor = MINOR (dev);
        printk (KERN_ERR "major = %d\n,minor = %d\n",major, minor);
    }
    //初始化cdev结构体对象
    cdev_init (&led_cdev, &led_fops);
    //注册cdev到内核
    cdev_add (&led_cdev, dev, 1);
 
    return 0;
}
void __exit led_drv_exit (void)
{
    //注销cdev对象
    cdev_del (&led_cdev);
    //注销设备
    unregister_chrdev_region (dev, 1);
    return ;
}
module_init (led_drv_init);
module_exit (led_drv_exit);
