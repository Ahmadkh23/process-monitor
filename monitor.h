#ifndef MONITOR_H
#define MONITOR_H

#include <stdio.h>
#include <sys/types.h>

typedef struct {
    pid_t pid;
    char name[256];
    char state;
    float cpu_usage;
    long memory;
} ProcessInfo;

// Function Prototypes
float get_total_cpu_usage(void);
void get_memory_usage(long *total_mem , long* free_mem);
int get_process_list(ProcessInfo *procs , int max_procs);
int kill_process(pid_t pid);
void log_action(const char *message);





#endif