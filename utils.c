#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"

float get_total_cpu_usage(void) {
    // Open /proc/stat in read-only ("r") mode
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return 0.0f;

    char buffer[1024];
    // fgets reads up to 1023 characters or until a newline from fp into buffer
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        fclose(fp);
        return 0.0f;
    }
    fclose(fp);

    char cpu_label[8];
    long user, nice, system, idle, iowait, irq, softirq;
    
    // sscanf parses data from a string buffer according to a specific format
    // Arguments: source buffer, format string, and pointers to variables where values will be saved.
    sscanf(buffer, "%s %ld %ld %ld %ld %ld %ld %ld", 
           cpu_label, &user, &nice, &system, &idle, &iowait, &irq, &softirq);

    // Calculate total time and idle time
    long total_idle = idle + iowait;
    long total_non_idle = user + nice + system + irq + softirq;
    long total = total_idle + total_non_idle;

    // Static variables persist across function calls. 
    // We need the delta (change) between two readings to compute current percentage.
    static long prev_total = 0;
    static long prev_idle = 0;

    long total_delta = total - prev_total;
    long idle_delta = total_idle - prev_idle;

    prev_total = total;
    prev_idle = total_idle;

    if (total_delta == 0) return 0.0f;

    // CPU % = (Total Time - Idle Time) / Total Time
    return ((float)(total_delta - idle_delta) / total_delta) * 100.0f;
}

void get_memory_usage(long *total_mem, long *free_mem) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return;

    char buf[256];
    *total_mem = 0;
    *free_mem = 0;

    // Loop through the file line by line
    while (fgets(buf, sizeof(buf), fp)) {
        // strncmp compares the first N characters of two strings. 0 means they match.
        if (strncmp(buf, "MemTotal:", 9) == 0) {
            // Parse the number directly out of the string buffer
            sscanf(buf, "MemTotal: %ld", total_mem);
        } else if (strncmp(buf, "MemFree:", 8) == 0) {
            sscanf(buf, "MemFree: %ld", free_mem);
        }
        
        // Optimize: break early if we have both values
        if (*total_mem > 0 && *free_mem > 0) break;
    }
    fclose(fp);
}