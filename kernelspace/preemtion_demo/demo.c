#include <linux/module.h>
#include <linux/kernel.h>	
#include <linux/moduleparam.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/kdev_t.h>
#include <linux/delay.h>
#include <linux/ioctl.h>
#include <linux/slab.h>
#include <linux/mempool.h>
#include <linux/mm.h>
#include <asm/io.h>


static int acme_count = 1;
static dev_t acme_dev;

static struct cdev *acme_cdev;


static ssize_t acme_read(struct file *file, char __user *buf,size_t count,loff_t *ppos)
{
   int len=5;
   if(*ppos > 0)
   {
	return 0;
   }
   mdelay(4000);
   cond_resched();
   mdelay(3000);
   if (copy_to_user(buf , "hello" , len)) {
      return -EFAULT;
   } else {
       *ppos +=len;
       return len;
   }
}



static struct file_operations acme_fops =
{
	.owner = THIS_MODULE,
	.read = acme_read,
};




static int hello_init (void)
{

	acme_dev = MKDEV(237,0);
	register_chrdev_region(acme_dev,1,"demo");



	acme_cdev=cdev_alloc();
	if(!acme_cdev)
	{
		printk (KERN_INFO "cdev alloc error.\n");
 		return -1;		
	}
	acme_cdev->ops = &acme_fops;
	acme_cdev->owner = THIS_MODULE;
	
	if(cdev_add(acme_cdev,acme_dev,acme_count))
	{
		printk (KERN_INFO "cdev add error.\n");
 		return -1;		
	}
	
 
  return 0;

}


static void
hello_cleanup (void)
{
	cdev_del(acme_cdev);
	unregister_chrdev_region(acme_dev, acme_count);
}


module_init (hello_init);
module_exit (hello_cleanup);
MODULE_LICENSE("GPL");
