#include "fpga.h"

static struct cdev *fpga_cdev;
static FPGA* myfpga;
static int fpga_dev_id;
static unsigned int counter;
static unsigned long fpga_addr;
static int irq;

static irqreturn_t *irq_handle(int irq, void * dev_id /*struct pt_regs*/ )
{
	if(*(int*) dev_id != fpga_dev_id)
		return IRQ_NONE;
	else
	{
		printk(KERN_INFO "interrupt occured!!\n");
		++counter;
		myfpga->CLEAR = 1;
		
		return (irqreturn_t*)IRQ_HANDLED;
	}
}

static ssize_t fpga_read(struct file *file, char __user *buf,size_t count,loff_t *ppos)
{
   int remaining_bytes;
    /* Number of bytes left to read in the open file */
   remaining_bytes = min(buf_size - (int) (*ppos), (int) count);
   if (remaining_bytes == 0) {
	  /* All read, returning 0 (End Of File) */
	  return 0;
   }
   if (copy_to_user(buf /* to */, *ppos+myfpga->REC /* from */, remaining_bytes)) {
      return -EFAULT;
   } else {
	  /* Increase the position in the open file */
      *ppos += remaining_bytes;
      return remaining_bytes;
   }
}

static int fpga_mmap(struct file *file,struct vm_area_struct* vma)
{
	int size;
	size=vma->vm_end - vma->vm_start;
	if(remap_pfn_range(vma,vma->vm_start,fpga_addr>>PAGE_SHIFT,size,vma->vm_page_prot))
		return -EAGAIN;
	return 0;
}

static int fpga_ioctl(struct inode *inode,struct file*file,unsigned int num,unsigned long param)
{
	int value = 0;
	switch(num)
	{
	 case 10 :myfpga->START = (int) param; break; //set start value
	 case 20: myfpga->END = (int) param ; break; //set stop value
	 case 30: myfpga->ENABLE = 1;break;//enable counter
	 case 40: myfpga->DISABLE = 1;break;//disable counter
	 case 50: value =  myfpga->DATA; break;//read current value				
	 case 60: myfpga->MATCH = (int) param; break;//set match value
	 case 70: value = counter; break; // get number of generated interrupts
	}
	return value;	
}

static struct file_operations fpga_fops =
{
	.owner = THIS_MODULE,
	.read = fpga_read,
	.mmap = fpga_mmap,
	.ioctl = fpga_ioctl,
};
static int __devinit fpga_probe(struct platform_device *pdev)
{
	struct resource *r;
	r = platform_get_resource(pdev, IORESOURCE_MEM , 0);
	fpga_addr= r->start;
	if((myfpga = ioremap(fpga_addr,0x1000) )< 0)
	{

 		printk (KERN_INFO "ioremap chrdev error.\n");
 		return -1;
	}
	r = platform_get_resource(pdev, IORESOURCE_IRQ , 0);
	irq = r->start;
	if(request_irq(irq, irq_handle, 0, "FPGA",&fpga_dev_id))
	{
		printk (KERN_INFO "request irq chrdev error.\n");
		return -1;
	}
	if(register_chrdev_region(fpga_dev,fpga_count,"FPGA"))
	{
 		printk (KERN_INFO "alloc chrdev error.\n");
 		return -1;
	}
	fpga_cdev=cdev_alloc();
	if(!fpga_cdev)
	{
		printk (KERN_INFO "cdev alloc error.\n");
 		return -1;		
	}
	fpga_cdev->ops = &fpga_fops;
	fpga_cdev->owner = THIS_MODULE;
	if(cdev_add(fpga_cdev,fpga_dev,fpga_count))
	{
		printk (KERN_INFO "cdev add error.\n");
 		return -1;		
	}
	printk (KERN_INFO "fpga loaded succefully.\n");
	return 0;
}

static int __devinit fpga_remove(struct platform_device *pdev)
{

	unregister_chrdev_region(fpga_dev,fpga_count);
	cdev_del(fpga_cdev);
	iounmap(myfpga);
    printk (KERN_INFO "fpga unloaded succefully.\n");
	return 0;
	
}

static struct platform_driver fpga_driver = {
		.remove = fpga_remove,
		.probe = fpga_probe,
		.driver = {
				.name = "myfpga",
				.owner = THIS_MODULE,
		},
};

static int __init fpga_init (void)
{
	platform_driver_register(&fpga_driver);
	return 0;
}

static void fpga_cleanup (void)
{
	platform_driver_unregister(&fpga_driver);

}
module_init (fpga_init);
module_exit (fpga_cleanup);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Elad Geva");
