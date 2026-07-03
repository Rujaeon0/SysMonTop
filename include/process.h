#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    long double memused;
    long double total_memory;
    long double available_memory;
    long double swap_total;
    long double swap_free;
    long pid;
    char processname[256];
} mem;

void get_mem_info(const char *path,mem *info2);
int process_scan(mem processes[]);
void pid_name(const char *path,mem *info2);
void total_memory_read(const char *path_meminfo,mem *info2);

#endif