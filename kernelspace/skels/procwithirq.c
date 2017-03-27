/*
 * rtc.c
 *
 *  Created on: Jan 23, 2011
 *      Author: root
 */
 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
 
MODULE_LICENSE("Dual BSD/GPL");
 
static int irq = 42;
struct class *my_class;

module_param(irq,int,0);
 
int * dev_id;
int flag = 0;
volatile int *rtc;
 
static irqreturn_t irq_handle(int x,void * dev_id)
{
    printk(KERN_DEBUG "Interrupt\n");
    rtc[7] = 0x1;
    rtc[4]=1;
    rtc[1] = rtc[0] +5;
   return 0;
}
static ssize_t
test_proc_read(struct file *file, char __user *buf,
                    size_t count, loff_t *ppos)
{
	printk("read test\n");
	return 0;
}

static ssize_t
test_proc_write(struct file *file, const char __user *buffer,
                     size_t count, loff_t *pos)
{
	printk("write test\n");

	return count;
}

static const struct file_operations test_proc_fops = {
	.owner = THIS_MODULE,
	.read  = test_proc_read,
	.write = test_proc_write,
	.llseek = noop_llseek,
};

 
static int acme_init (void)
{
    struct proc_dir_entry *entry;
    int req_irq = request_irq(irq, irq_handle, 0, "mydev", &dev_id);
    entry = proc_create("mytest", 0, NULL, &test_proc_fops);

    printk(KERN_DEBUG "init %d\n",req_irq); 
    rtc=ioremap(0x101e8000,0x1000);
    rtc[4]=1;
    rtc[1] = rtc[0] +5;
    return 0;
}
 
static void hello_cleanup (void)
{
    printk (KERN_DEBUG "Module unloaded succefully.\n");
    free_irq(irq, &dev_id);
    remove_proc_entry("mytest", NULL);
}
 
module_init (acme_init);
module_exit (hello_cleanup);
