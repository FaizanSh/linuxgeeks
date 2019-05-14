#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kprobes.h>//To make use of Kprobe functionality, you must declare a variable of the structure struct kprobe, which is declared in include/linux/kprobes.h. Here’s a little extract
#include <linux/kallsyms.h>
#include <linux/fdtable.h>
#include <linux/sched.h>
#include <linux/string.h> 
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/futex.h>

char *mycommand = "mysqld";
static int myskipcount = 0;


module_param(mycommand, charp, 0);
MODULE_PARM_DESC(mycommand, "Name of program to monitor, Use this to get pid. Default is \"mysqld\"");
module_param(myskipcount, int, 0);
MODULE_PARM_DESC(myskipcount, "How many symbols to skip before taking action. Default is 0");

static unsigned int counter = 0;
int pre_futexhandler(struct kprobe *p, struct pt_regs *regs){
    
    unsigned char myinternalcommand[sizeof(current->comm)];	
    get_task_comm(myinternalcommand, current);
	// printk("Entered in Futex handler: \n");
	//printk("Get tast comm is  %s\n",myinternalcommand);
    if (strstr(myinternalcommand, mycommand) != NULL){
	//printk("Entered in If NOT NULL: \n");
	//if (op == FUTEX_WAKE || op == FUTEX_WAKE_OP || op == FUTEX_WAKE_PRIVATE || op == FUTEX_WAKE_OP_PRIVATE) {
	    counter++;
	    printk("KProbefutex: futex called for %s %u times\n",myinternalcommand,counter);
	    if (counter > myskipcount){
	    	printk("Sending Kill signal to %s\n",myinternalcommand);
            	send_sig(SIGKILL,current,0);
	    }		
	}
    //jprobe_return();
    return 0;
    }
    

void Post_Handler(struct kprobe *p, struct pt_regs *regs, unsigned long flags){ 
   // printk("Post_Handler: counter=%u\n",counter);
} 
 
 
 
static struct kprobe kp_futex;
 
int myinit(void)
{
    printk("Kprobefutex module inserted\n ");
    printk("Program to monitor is      : %s\n",mycommand);
    printk("Kernel Func to monitor     : sys_futex\n");
    printk("Calls to Skip before Action: %u\n",myskipcount);
	
	kp_futex.pre_handler = pre_futexhandler; 
    kp_futex.post_handler = Post_Handler; 
    
	kp_futex.addr = (kprobe_opcode_t *) kallsyms_lookup_name("sys_futex"); //instruction pointer return
 //   kp_futex.entry = (kprobe_opcode_t *) pre_futexhandler;
    register_kprobe(&kp_futex);
    return 0;
}
 
void myexit(void)
{
    unregister_kprobe(&kp_futex);
    printk("kprobefutex module removed\n ");
}
 
module_init(myinit);
module_exit(myexit);
MODULE_AUTHOR("Anjum Naveed Edited by Faizan Ullah");
MODULE_DESCRIPTION("It was JPROBE MODULE FOR FUTEX TRAP Now its just kprobe");
MODULE_LICENSE("GPL");

