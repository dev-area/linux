/*
 * Simple - REALLY simple memory mapping demonstration.
 *
 * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2001 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 * $Id: simple.c,v 1.12 2005/01/31 16:15:31 rubini Exp $
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/mm.h>
#include <linux/kdev_t.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/gfp.h>
#include <linux/cdev.h>
#include <linux/device.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Jonathan Corbet");

static int irq=20;
module_param(irq,int,0660);
static struct proc_dir_entry *ent;
static ssize_t myread(struct file *file, char __user *buf,size_t count, loff_t *ppos) 
{
	if(*ppos > 0)
		return 0;
	copy_to_user(buf,"hello\n",6);
	*ppos = 6;
	return 6;
}
static struct file_operations myops = 
{
	.read = myread,
};
static int simple_init(void)
{
	ent=proc_create("mytest",0660,NULL,&myops);
	printk(KERN_ALERT "hello, module %d...\n",irq);
	return 0;
}


static void simple_cleanup(void)
{
	proc_remove(ent);
	printk(KERN_WARNING "bye  module %d...\n",irq);
}


module_init(simple_init);
module_exit(simple_cleanup);







