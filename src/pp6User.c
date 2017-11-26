#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>


typedef struct memInfo {
    long int pid;
    long int vms;
    long int rss;
    long int min_flt;
    long int maj_flt;
} memInfo;

int main(int argc, char *argv[]) {
    memInfo **user = malloc(sizeof(struct memInfo *));
    
    // Setup array of memInfo structs from user input
    int i, pid;
    for (i = 0; i < argc; i++) {
        pid = atoi(argv[i]);
        memInfo temp = (memInfo){ pid, 0, 0, 0, 0 };
        user[i] = &temp;
    }

    // Make syscall
    long int ret_status = syscall(328, (void *)user, sizeof(struct memInfo *) * argc);

    if (ret_status == 0) {
        memInfo **ret = (struct memInfo *)user;

        // Print out each process' memory info
        for (i = 0; i < argc; i++) {
            printf("PID: %ld\n", ret[i]->pid);
            printf("MajFlts: %ld  MinFlts: %ld\n", ret[i]->maj_flt, ret[i]->min_flt);
            printf("VM: %ld  RSS: %ld\n", ret[i]->vms, ret[i]->rss);
        }
    }
    else {
        printf("Syscall failed! Return status: %ld\n", ret_status);
    }


    return 0;
}