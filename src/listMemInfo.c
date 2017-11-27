#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

#include "memInfo.h"

typedef struct memInfo {
    long int pid;
    long int vms;
    long int rss;
    long int min_flt;
    long int maj_flt;
    long int state;
    long int sys_time;
    long int user_time;
} memInfo;

struct memInfo **listMem;

void dfs(struct task_struct *task, int size) {
    struct task_struct *child;
    struct list_head *list;

    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);
        dfs(child, size);

        int proc_pid = (long)task_pid_nr(child);
        int i;
        for (i = 0; i < size; i++) {
            if (proc_pid == listMem[i]->pid) {
                // Set proc info for struct
                listMem[i]->vms = get_mm_hiwater_vm(child->mm) * 4;
                listMem[i]->rss = get_mm_hiwater_rss(child->mm) * 4;
                listMem[i]->min_flt = child->min_flt;
                listMem[i]->maj_flt = child->maj_flt;
                listMem[i]->state = child->state;
                listMem[i]->sys_time = child->stime;
                listMem[i]->user_time = child->utime;
                printk("PID: %ld\n", proc_pid);
                printk("MajFlts: %ld  MinFlts: %ld\n", child->maj_flt, child->min_flt);
                printk("VM: %ld  RSS: %ld\n", get_mm_hiwater_vm(child->mm), get_mm_hiwater_rss(child->mm));
                printk("State: %ld System Time: %ld User Time: %ld\n", child->state, child->stime, child->utime);
                break;
            }
        }
    }
}

asmlinkage long sys_listMemInfo(void **user, int size) {
    printk("listMemInfo sysCall!\n");
    listMem = (struct memInfo *)user;

    int list_size = size / sizeof(struct memInfo *);

    int i;
    for (i = 0; i < list_size; i++) {
        printk("PID #%i: %i\n", i, listMem[i]->pid);
    }

    dfs(&init_task, list_size);

    // Return listMem to user
    int ret = copy_to_user(user, (void *)listMem, sizeof(struct listMem *));
    if (ret) {
        printk("Error! %d\n", ret);
    }

    return 0;
}