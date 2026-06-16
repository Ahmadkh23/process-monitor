#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <unistd.h>
#include "monitor.h"

void display_ui(void) {
    long total_mem, free_mem;
    ProcessInfo procs[128]; 

    printf("\033[H\033[J");

    printf("            MINI LINUX PROCESS MONITOR         \n");

    // Get and print Global System Stats
    float cpu = get_total_cpu_usage();
    get_memory_usage(&total_mem, &free_mem);
    long used_mem = total_mem - free_mem;

    printf("CPU Usage:    [%.2f%%]\n", cpu);
    printf("Memory Usage: [%ld MB / %ld MB] (%.1f%% used)\n", 
            used_mem / 1024, total_mem / 1024, ((float)used_mem / total_mem) * 100);
    printf("--------------------------------------------------\n");

    // Fetch and print processes
    int num_procs = get_process_list(procs, 128);
    printf("%-8s %-20s %-8s %-12s\n", "PID", "NAME", "STATE", "RES MEM (KB)");
    printf("--------------------------------------------------\n");

    // Display top 15 processes for UI layout clarity
    int display_limit = (num_procs > 15) ? 15 : num_procs;
    for (int i = 0; i < display_limit; i++) {
        printf("%-8d %-20s %-8c %-12ld\n", 
               procs[i].pid, procs[i].name, procs[i].state, procs[i].memory);
    }

    printf("--------------------------------------------------\n");
    printf("Options: ./process_monitor kill [PID] | Updates every 2s\n");
}

// Fixed: Only ONE main function that accepts arguments cleanly
int main(int argc, char *argv[]) {
    // If user passed arguments like: ./process_monitor kill 1234
    if (argc == 3 && strcmp(argv[1], "kill") == 0) {
        pid_t target = atoi(argv[2]);
        if (kill_process(target) == 0) {
            char buf[64];
            snprintf(buf, sizeof(buf), "Successfully killed PID %d", target);
            log_action(buf);
            printf("Process %d terminated.\n", target);
        } else {
            printf("Error: Could not kill process %d.\n", target);
        }
        return 0;
    }

    // Otherwise, boot the monitor loop
    setvbuf(stdout, NULL, _IONBF, 0);
    get_total_cpu_usage();
    log_action("Process Monitor dynamic UI loop initiated.");

    while (1) {
        display_ui();
        sleep(2);
    }
    return 0;
}