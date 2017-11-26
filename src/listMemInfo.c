#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

typedef struct memInfo {
    long int pid;
    long int vms;
    long int rss;
    long int min_flt;
    long int maj_flt;
} memInfo;

struct memInfo *listMem[];

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
                listMem[i]->vms = get_mm_hiwater_vm(child->mm);
                listMem[i]->rss = get_mm_hiwater_rss(child->mm);
                listMem[i]->min_flt = child->min_flt;
                listMem[i]->maj_flt = child->maj_flt;
                printk("PID: %ld\n", proc_pid);
                printk("MajFlts: %ld  MinFlts: %ld\n", child->maj_flt, child->min_flt);
                printk("VM: %ld  RSS: %ld\n", get_mm_hiwater_vm(child->mm), get_mm_hiwater_rss(child->mm));
                break;
            }
        }
    }
}

asmlinkage long sys_listMemInfo(void **user[], int size) {
    printk("listMemInfo sysCall!\n");
    listMem = (struct memInfo *)user;

    int list_size = sizeof(listMem) / sizeof(struct memInfo *);

    int i;
    for (i = 0; i < list_size; i++) {
        printk("PID #%i: %i\n", i, listMem[i]->pid);
    }

    dfs(&init_task, list_size);

    // Return listMem to user
    int ret = copy_to_user(user, (void *)listMem, sizeof((struct listMem) * size));
    if (ret) {
        printk("Error! %d\n", ret);
    }

    return 0;
}