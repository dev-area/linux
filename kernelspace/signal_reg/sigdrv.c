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

#include "sigreg.h"

static int pids[100];
static int pos = 0;
static int regsig = 35; 

static int bc_signal(int param)
{
 	int ret,i;
	struct siginfo info;
	struct task_struct *t;

	for(i=0;i<pos;i++)
	{
		memset(&info, 0, sizeof(struct siginfo));
		info.si_signo = regsig;
		info.si_code = SI_QUEUE;
		info.si_int = param;

		rcu_read_lock();
		t = pid_task(find_pid_ns(pids[i], &init_pid_ns), PIDTYPE_PID);	
		if(t == NULL){
			pr_err("no such pid %d\n", pids[pos]);
			rcu_read_unlock();
			continue;
		}
		
		rcu_read_unlock();

		ret = send_sig_info(regsig, &info, t);    //send the signal
		if (ret < 0) {
			pr_err("error sending signal to pid=%d\n",pids[pos]);
		}
	}
	return 0;
 
}



static int add_process(void)
{
	if(current != NULL && pos < 99)
	{
		pids[pos] = current->tgid;
		if(pos < 99)
			pos++;
		return 0;
	}
	return -1;
} 
		
static int set_signal(int signum)
{
	if (signum > 0 && signum < 65)
	{
		regsig = signum;
		return 0;
	}
	return -1;
}


static int sig_ioctl(struct file* file,unsigned int num,unsigned long param)
{
	
	switch(num)
	{
		case SIGNAL_REGISTER:
				return add_process();
		case SIGNAL_SETSIG:
				return set_signal(param);
		case SIGNAL_BROADCAST:
				return bc_signal(param);
	}		
	return 0;
}


static struct file_operations sig_fops =
{
	.owner = THIS_MODULE,
	.unlocked_ioctl = sig_ioctl,
};



static int sigdev_count = 1;
static dev_t sig_dev = MKDEV(500,1);

static struct cdev *sig_cdev;


static int
hello_init (void)
{
	if(register_chrdev_region(sig_dev,sigdev_count,"sigtest"))
	{
 		printk (KERN_INFO "alloc chrdev error.\n");
 		return -1;
	}
	
	sig_cdev=cdev_alloc();
	if(!sig_cdev)
	{
		printk (KERN_INFO "cdev alloc error.\n");
 		return -1;		
	}
	sig_cdev->ops = &sig_fops;
	sig_cdev->owner = THIS_MODULE;
	
	if(cdev_add(sig_cdev,sig_dev,sigdev_count))
	{
		printk (KERN_INFO "cdev add error.\n");
 		return -1;		
	}
	
 
  return 0;

}


static void
hello_cleanup (void)
{
    cdev_del(sig_cdev);
    unregister_chrdev_region(sig_dev, sigdev_count);
	
    printk (KERN_INFO "hello unloaded succefully.\n");

}

 
module_init (hello_init);
module_exit (hello_cleanup);
MODULE_LICENSE("GPL");
