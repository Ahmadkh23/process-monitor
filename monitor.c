#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <ctype.h>
#include "monitor.h"

// Helper function to check if a directory name is purely numeric (a PID)
static int is_pid_dir(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isdigit(name[i])) return 0; // Not a number
    }
    return 1; // It is a number
}

int get_process_list(ProcessInfo *procs, int max_procs) {
    // opendir opens a directory stream corresponding to the directory path string
    DIR *dir = opendir("/proc");
    if (!dir) return 0;

    struct dirent *entry;
    int count = 0;

    // readdir reads the next directory entry from the stream sequentially. Returns NULL at end.
    while ((entry = readdir(dir)) != NULL && count < max_procs) {
        if (entry->d_type != DT_DIR || !is_pid_dir(entry->d_name)) {
            continue; // Skip files or non-PID directories
        }

        // Convert string directory name to integer PID using atoi (ASCII to Integer)
        pid_t pid = atoi(entry->d_name);
        
        // Construct the path to the process status file: /proc/[PID]/stat
        char stat_path[512];
        // snprintf safe-formats strings into a bounded array buffer to prevent buffer overflows
        snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);

        FILE *fp = fopen(stat_path, "r");
        if (!fp) continue;

        ProcessInfo p;
        p.pid = pid;

        // /proc/[pid]/stat format is: PID (Name) State ...
        // %*d means read an integer but discard it.
        // %[^)] parses strings until it encounters a ')' character (handles spaces in process names!)
        char state_str[2];
        long rss;
        
        // Reading positional tokens based on Linux man-pages for /proc/[pid]/stat
        if (fscanf(fp, "%*d (%[^)]) %s %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*u %*u %ld", 
                   p.name, state_str, &rss) >= 3) {
            
            p.state = state_str[0];
            // RSS (Resident Set Size) is given in pages. Typically, 1 page = 4KB.
            p.memory = rss * 4; 
            p.cpu_usage = 0.0f; // Simplified for basic polling interval

            procs[count] = p;
            count++;
        }
        fclose(fp);
    }
    closedir(dir); // Close directory stream to prevent system leaks
    return count;  // Return number of processes successfully found
}

int kill_process(pid_t pid) {
    // kill() is a POSIX system call. 
    // Argument 1: target PID. Argument 2: Signal integer. SIGKILL (9) forces instant termination.
    if (kill(pid, SIGKILL) == 0) {
        return 0; // Success
    }
    return -1; // Failure (e.g., process doesn't exist or permission denied)
}