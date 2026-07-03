#ifndef CPU_H
#define CPU_H

typedef struct {
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long iowait;
    unsigned long irq;
    unsigned long softirq;
    unsigned long steal;
    double cpu_usage;
} CPUInfo;

void get_cpu_info(CPUInfo *info1);

#endif