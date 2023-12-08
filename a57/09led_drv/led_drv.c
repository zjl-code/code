#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/acpi.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
MODULE_LICENSE ("GPL");
//定义一个cdev对象
struct cdev led_cdev;
//定义class类的对象
static struct class*led_class;
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
ssize_t led_read (struct file*filp, char __user *buf, size_t count, loff_t *ppos)
{
    printk (KERN_ERR "enter:%s\n", __func__);
    return 0;
}
ssize_t led_write (struct file*filp, const char __user *buf, size_t count, loff_t *ppos)
{
    printk (KERN_ERR "enter:%s\n", __func__);
    return 0;
}

struct file_operations led_fops = //定义了file_operations结构体类型的对象led_fops
{
    .owner = THIS_MODULE,
    .open = led_open,
    .release = led_close, 
    .write = led_write,
    .read = led_read,
};
int __init led_drv_init (void)
{
    major = register_chrdev (0, "dev_led", &led_fops);
    //创建设备树的树枝
    led_class = class_create (THIS_MODULE, "led");
    //创建设备树的果实
    device_create (led_class, NULL, MKDEV(major, 0), NULL, "led_gpio5_0");
    
    return 0;
}
void __exit led_drv_exit (void)
{
    //注销设备文件
    device_destroy (led_class, MKDEV (major, 0));
    //注销设备类
    class_destroy (led_class);
    //注销设备号
    unregister_chrdev (major, "dev_led");
       return ;
}
module_init (led_drv_init);
module_exit (led_drv_exit);
