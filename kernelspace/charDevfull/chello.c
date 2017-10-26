/*
 * $Id: hello2.c,v 1.8 2001/07/18 22:28:18 rubini Exp $
 */

#include <linux/module.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/moduleparam.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/device.h>


static void *buffer1;
static void *buffer2;

static int buf_size;

static ssize_t
acme_read(struct file *file, char __user *buf,size_t count,loff_t *ppos)
{
 	int remaining_bytes;
	void *buffer;
 	
 	int minor = MINOR(file->f_path.dentry->d_inode->i_rdev);
 	if(minor == 1)
 		buffer = buffer1;
 	else
 		buffer = buffer2;
   /* Number of bytes left to read in the open file */
	printk("read callback %d\n",minor);
	
   remaining_bytes = min(buf_size - (int) (*ppos), (int) count);
   
   if (remaining_bytes == 0) {
	  /* All read, returning 0 (End Of File) */
	  return 0;
   }

   if (copy_to_user(buf /* to */, *ppos+buffer /* from */, remaining_bytes)) {
      return -EFAULT;
   } else {
	  /* Increase the position in the open file */
      *ppos += remaining_bytes;
      return remaining_bytes;
   }
}

static ssize_t
acme_write(struct file *file,const char __user *buf,size_t count,loff_t *ppos)
{
  int remaining_bytes;
  void *buffer;
  
 	if ( current-> tgid == 1)
 		return -1;
 		
 	int minor = MINOR(file->f_path.dentry->d_inode->i_rdev);
 	if(minor == 1)
 		buffer = buffer1;
 	else
 		buffer = buffer2;

	printk("write callback\n");
   /* Number of bytes not written yet in the device */
   remaining_bytes = buf_size - (*ppos);
   
   if (count > remaining_bytes) {
	  /* Can't write beyond the end of the device */
	  return -EIO;
   }

   if (copy_from_user(*ppos+buffer /* to */, buf /* from */, count)) {
      return -EFAULT;
   } else {
	  /* Increase the position in the open file */
      *ppos += count;
      return count;
   }

}
static struct file_operations acme_fops =
{
	.owner = THIS_MODULE,
	.read = acme_read,
	.write = acme_write,
};



static int acme_count = 10;
static dev_t acme_dev;

static struct cdev *acme_cdev;

static struct class *mtc;
static struct device *dev1;
static struct device *dev2;


static int
hello_init (void)
{

mtc = class_create(THIS_MODULE, "myclass");
	

dev1 = device_create(mtc, NULL, MKDEV(340, 1), NULL, "myclip1");
dev2 = device_create(mtc, NULL, MKDEV(340, 2), NULL, "myclip2");
			    
	buffer1 = kzalloc(8192,GFP_USER);
	buffer2 = kzalloc(8192,GFP_USER);
	
	buf_size = 8192;
	if(!buffer1)
	{
 		printk (KERN_INFO "kzalloc error.\n");
 		return -1;
	}
	if(register_chrdev_region(MKDEV(340,1),2,"clip"))
	{
 		printk (KERN_INFO "alloc chrdev error.\n");
 		return -1;
	}
	
	acme_cdev=cdev_alloc();
	if(!acme_cdev)
	{
		printk (KERN_INFO "cdev alloc error.\n");
 		return -1;		
	}
	acme_cdev->ops = &acme_fops;
	acme_cdev->owner = THIS_MODULE;
	
	if(cdev_add(acme_cdev,MKDEV(340,1),2))
	{
		printk (KERN_INFO "cdev add error.\n");
 		return -1;		
	}
	
 
  return 0;

}


static void
hello_cleanup (void)
{
  kfree(buffer1);
  kfree(buffer2);
  cdev_del(acme_cdev);
  unregister_chrdev_region(MKDEV(340,1),2);
  printk (KERN_INFO "hello unloaded succefully.\n");

}


module_init (hello_init);
module_exit (hello_cleanup);
MODULE_LICENSE("GPL");
