#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

MODULE_LICENSE ("GPL");
unsigned long pages_addr = 0 ;
void *kmalloc_addr = 0;
void *vmalloc_addr = 0;

int __init kernelspace_init (void)
{
    int err = 0;
    //申请了8个物理内存页
    pages_addr = __get_free_pages (GFP_KERNEL, 3);
    if (!pages_addr)
    {
        err = -ENOMEM;
        goto failuer_get_free_pages;
    }
    printk ( KERN_ERR "pages virt address = %p phy addr = %p\n", pages_addr, virt_to_phys(pages_addr));
   kmalloc_addr =  kmalloc (100, GFP_KERNEL);
   if (!kmalloc_addr)
   {
        err = - ENOMEM;
        goto failuer_kmalloc;
   }
printk ( KERN_ERR "kmalloc virt address = %p phy addr = %p\n", kmalloc_addr, virt_to_phys(kmalloc_addr));

   vmalloc_addr = vmalloc (10*1024);//10k
   if (!vmalloc_addr)
   {
        err = -ENOMEM;
        goto failuer_vmalloc;
   }
printk ( KERN_ERR "vmalloc virl address = %p phy addr = %p\n", vmalloc_addr, virt_to_phys(vmalloc_addr));

failuer_vmalloc:
    vfree (vmalloc_addr);
failuer_kmalloc:
    kfree (kmalloc_addr);
failuer_get_free_pages:
    free_pages (pages_addr, 3);
    return err;
    
}

void __exit kernelspace_exit (void)
{
    vfree (vmalloc_addr);
    kfree (kmalloc_addr);
    free_pages (pages_addr, 3);
}
module_init (kernelspace_init);
module_exit (kernelspace_exit);




