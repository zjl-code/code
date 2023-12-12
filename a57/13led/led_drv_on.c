#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/acpi.h>
#include <linux/kernel.h>
MODULE_LICENSE ("GPL");

#define X0_REG 0x32b3015c
#define GPIO_DDR 0x28039004
#define GPIO_DR 0x28039000

int k_status = 1;
//定义一个cdev对象
struct cdev led_cdev;
//定义class类的对象
static struct class*led_class;
dev_t dev;//保存设备号
static int major = 0;//主设备号
unsigned int minor = 10;//次设备号
 //配置GPIO5_0为输出模式
 int led_open (struct inode *inode, struct file *filp )
{
    void __iomem *vaddr;
    //配置GPIO管脚模式
    vaddr = ioremap(X0_REG, 8);
    writel (0x246, vaddr);
    //gpio5_0输出模式
   vaddr = ioremap (GPIO_DDR, 8);
   writel (0x00000001 | readl (vaddr),vaddr);
    return 0;
}
int led_close (struct inode *inode, struct file *filp )
{
    printk (KERN_ERR "enter:%s\n", __func__);
    return 0;
}
ssize_t led_read (struct file *filp,char __user *buf, size_t count, loff_t *ppos)
{
    int ret = 0;
    ret = copy_to_user (buf, &k_status, sizeof (int));
    return (ssize_t) ((ret == 0) ? count : -1);
}
ssize_t led_write (struct file *filp,const char __user *buf, size_t count, loff_t *ppos)
{
    char val = 255;
    int ret = 0;
    void __iomem* vaddr;
    //从用户态拷贝到内核态，从buf拷贝到val中
    ret = copy_from_user(&val, buf, count);
    vaddr = ioremap (GPIO_DR, 4);
    if (val&0x01 )
    {
        writel (readl (vaddr) | 0x01, vaddr);
    }
    else 
    {
        writel (readl (vaddr) & 0xfffffffe, vaddr);
    }
    val = readb (vaddr) &0x01;//读取最低位
    k_status = val;

    return (ssize_t) ((ret == 0)?count:-1);

}
static struct file_operations led_fops = //定义了file_operations结构体类型的对象led_fops
{
    .owner = THIS_MODULE,//在本模块生效
    .open = led_open,
    .release = led_close, 
    .read = led_read,
    .write = led_write,
};
int __init led_drv_init (void)
{   
    
    //注册设备
    major = register_chrdev (0, "dev_led", &led_fops);
    //设备文件的自动创建
    //创建设备树的树枝
    led_class = class_create (THIS_MODULE, "uleds");
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
