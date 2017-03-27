#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <asm/io.h>


static void got_char(void *scancode)
{
	printk(KERN_ALERT"Scan Code %x %s.\n",
		(int) *((char *) scancode) & 0x7F,
		*((char *) scancode) & 0x80 ? "Released" : "Pressed");
}

static unsigned char scancode;

DECLARE_TASKLET(module_tasklet, got_char, &scancode);

irqreturn_t irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
	unsigned char status;


	/* Read keyboard status */
	status = inb(0x64);
	scancode = inb(0x60);
	tasklet_schedule(&module_tasklet);

	/* Scheduale bottom half to run */
	return IRQ_HANDLED;
}
int init_module()
{
	int result;
	result = request_irq(1,irq_handler,
				SA_SHIRQ,"test_keyboard_irq_handler",
					(void *)(irq_handler));

	if ( result != 0 )
	{
		printk ( KERN_ALERT"error, result=%d\n",result );
		return -1;
	}
	else
	{
		printk ( KERN_ALERT"succes!\n" );
	return 0;
	}

}
void cleanup_module()
{
	free_irq(1, NULL);
}
