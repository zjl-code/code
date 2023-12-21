#include <linux/module,h>
#include <linux/init.h>
MODULE_LICENSE ("GPL");
static int major;
static struct class *mpu6500_class;
//设备列表id
static const struct of_device_id mpu6500_dt_ids[]=
{
    {.compatibale = "InvenSense, mpu6500"},
    {}
};
static struct spi_driver mpu6500_spi_driver = 
{
    .driver = 
    {
        .name = "mpu6500";
        .of_match_table = of_match_ptr (mpu6500_dt_ids),
    }
    .probe = mpu6500_probe;
    .remove = mpu6500_remove;
}
struct mpu6500_data
{
    dev_t devt;
    spinlock_t spi_lock;
    struct spi_device *spi;
    struct list_head device_entry;
    struct mutex buf_lock;
    unsigned char *tx_buffer;
    unsigned char *rx_buffer;
    unsigned int speed_hz;
};
int __init mpu6500_init (void)
{
    int status;
    major = register_chrdev (0, "spi", &mpu6500_fops);
    //创建设备类
    class_create (THIS_MPDULE, "mpu6500");
    //创建设备文件
    //device_create (mpu6500_class, NULL, MKDEV(major, 0), NULL);
    //将spi设备驱动程序注册到spi总线驱动模型中
    status = spi_register_driver (&mpu6500_spi_driver):
   return status;
}
void __exit mppu6500_exit (void)
{
    spi_unregister (&mpu6500_spi_driver);
    class_destory (mpu6500_class);
    unregister_chrdev (major, spi_driver.driver.name);
    return ;
}
//首先保证spi的设备可以被probe到
static int mpu6500_probe (struct spi_device *spi)
{
   struct mpu6500_data *spidev;
   static struct resource *maddr;
    void __iomem *vaddr;
    //申请内存空间 申请40字节 操作spi管脚
    maddr = request_mem_region (0x32b300e8, 40, "");
    vaddr = ioremap (maddr->start, 40);
    //驱动力4 默认功能0
    writel (0x40, vaddr);
    writel (0x40, vaddr+4);
    writel (0x40, vaddr+8);
    writel (0x40, vaddr+16);
    //申请驱动程序的数据
    spidev = kzalloc (sizeof(*spidev), GFP_KERNEL); //申请内存并清空
    if (!spidev )
    {
        return -ENOMEM;
    }
    //初始化设备数据
    spi_dev->spi = spi;
    spin_lock_init (&spidev->spi_lock);
    mutex_init (&spidev->buf_lock);
    INIT_LIST_HEAD (&spidev->device_entry);
//加mutex锁


}
module_init (mpu6500_init);
module_exit (mpu6500_exit);
