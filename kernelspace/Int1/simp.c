#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/wait.h>

MODULE_LICENSE("Dual BSD/GPL");

static irq = 12;
module_param(irq,int,0);

int * dev_id;
int flag = 0;

static int count=0;

static irqreturn_t *irq_handle(void * dev_id)
{
	printk(KERN_DEBUG "Interrupt %d\n",count++);
	return 0;
}

static int acme_init (void)
{
	int req_irq = request_irq(irq, irq_handle, SA_SHIRQ, "mydev", &dev_id);
	printk(KERN_WARNING "%s init\n", current->comm);
    return 0;
}

static void hello_cleanup (void)
{
	printk (KERN_DEBUG "Module unloaded succefully.\n");
	free_irq(irq, &dev_id);

}

module_init (acme_init);
module_exit (hello_cleanup); 
