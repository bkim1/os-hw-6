#include <linux/sched.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/fs.h>


int size;
int pids[4];
module_param_array(pids, int, &size, 0);

int pp6_init(void) {
    printk("Loading PP6 Module\n");
    int i, len, read;
    char *line;
    for (i = 0; i < size; i++) {
        printk("PID #%i: %i\n", i, pids[i]);
        struct file *status = filp_open("/proc/" + pids[i] + "/status", O_RDONLY, 0);
        if (status == NULL) { exit(1); }
        
        while((read = getline(&line, &len, status)) != -1) {  
            printk("%s\n", line);   
            char *ptr = strchr(line, ':');
            if (ptr) {
                int index;
                index = ptr - line;
            }
        }
    }
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