#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>


int size;
int pids[4];
module_param_array(pids, int, &size, 0);

void dfs(struct task_struct *task) {
    struct task_struct *child;
    struct list_head *list;

    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);
        dfs(child);

        int proc_pid = (long)task_pid_nr(child);
        int i;
        for (i = 0; i < size; i++) {
            if (proc_pid == pids[i]) {
                printk("PID: %ld\n", proc_pid);
                printk("MajFlts: %ld  MinFlts: %ld\n", child->maj_flt, child->min_flt);
                printk("VM: %ld  RSS: %ld\n", get_mm_hiwater_vm(child->mm), get_mm_hiwater_rss(child->mm));
                break;
            }
        }
        
    }
}



int pp6_init(void) {
    printk("Loading PP6 Module\n");
    int i;
    for (i = 0; i < size; i++) {
        printk("PID #%i: %i\n", i, pids[i]);
    }

    dfs(&init_task);

    return 0;
}

void pp6_exit(void) {
    printk("Removing module...\n");
}

module_init(pp6_init);
module_exit(pp6_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PP6");
MODULE_AUTHOR("BKIM");