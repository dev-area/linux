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
#include <linux/delay.h>
#include <asm/page.h>
#include <linux/gfp.h>
#include <linux/cdev.h>
#include <linux/device.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Jonathan Corbet");


static int simple_init(void)
{
	printk(KERN_ALERT "hello...\n");
	return 0;
}


static void simple_cleanup(void)
{
	printk(KERN_WARNING "bye ...\n");
}


module_init(simple_init);
module_exit(simple_cleanup);
