#include <linux/module.h>
#include <linux/skbuff.h>          
#include <linux/init.h>
#include <net/sock.h>
#include <linux/inet.h>
#include <linux/ip.h>             
#include <linux/kernel.h> 
#include <linux/netfilter.h>
#include <uapi/linux/netfilter_ipv4.h> 

 
unsigned int main_hook(unsigned int hooknum,
                       struct sk_buff *skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int (*okfn)(struct sk_buff*))
{
    struct iphdr *iph;
 
    iph = ip_hdr(skb);
    if(iph->saddr == in_aton("192.168.0.2"))
    { 
        return NF_DROP; 
    }
    return NF_ACCEPT;
}

static struct nf_hook_ops netfops;                    

int __init my_module_init(void)
{
    netfops.hook              =       main_hook;
    netfops.pf                =       PF_INET;        
    netfops.hooknum           =       0;
    netfops.priority          =       NF_IP_PRI_FIRST;
    nf_register_hook(&netfops);
 
    return 0;
}
void __exit my_module_exit() 
{ 
    nf_unregister_hook(&netfops); 
}
 
module_init(my_module_init);
module_exit(my_module_exit);
MODULE_LICENSE("GPL");

