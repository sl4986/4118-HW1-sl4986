#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/list.h>
#include <linux/printk.h>
void find (struct task_struct *task, int layer){
	struct task_struct * it;
	struct list_head *p;
	list_for_each(p, &task->children){
		it=list_entry(p,struct task_struct, sibling);printk(KERN_INFO"%*s\\_[%d]:%s",4*min(layer,20),"",it->pid,it->comm); find (it,layer+1);}
	}

int tasklist_init(void)
{
	printk(KERN_INFO "Loading Module\n");
	printk(KERN_INFO "--[%d]%s\n", init_task.pid, init_task.comm);
	find (&init_task,0);


	return 0;
}

void tasklist_exit(void)
{
	printk(KERN_INFO "Removing Module\n");
}

module_init( tasklist_init );
struct list_head children, sibling;
module_exit( tasklist_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Tasklist Module");
MODULE_AUTHOR("SGG");
