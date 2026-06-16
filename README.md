# Linux Process Monitor

A lightweight, high-performance command-line process monitor (mini-htop) written in pure C. This tool interacts directly with the Linux kernel by parsing the virtual `/proc` filesystem to stream live system metrics and process state states.

##  Features
* **Live CPU Telemetry:** Tracks real-time global CPU usage percentages via `/proc/stat`.
* **Memory Tracking:** Monitors system-wide physical memory allocation from `/proc/meminfo`.
* **Process Enumeration:** Lists running PIDs, process names, execution states, and resident set size (RSS) memory.
* **Process Signalling:** Directly terminates target tasks using POSIX signals (`SIGKILL`).
* **Activity Logging:** Records application life-cycle and diagnostic kill events to a local file stream.

##  Architecture
* `main.c` - Core runtime execution and console UI dashboard layout.
* `monitor.c` - `/proc` directory traversal and process manipulation.
* `utils.c` - Low-level parsing routines for global CPU and memory states.
* `logger.c` - Timestamped operational logging.
* `monitor.h` - System data structures and function prototypes.

##  Compilation & Usage
> **Note:** This project relies on Linux kernel internals and must be executed in a Linux environment (Ubuntu, Debian, Docker, or WSL).

Compile the source using the provided `Makefile`:
make
Run the live dashboard:
Bash
./process_monitor
Kill a specific process:
Bash
./process_monitor kill <PID>
Clean build artifacts:
Bash
make clean
