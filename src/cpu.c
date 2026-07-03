#include "cpu.h"
#include <stdio.h>
#include <unistd.h>

void get_cpu_info(CPUInfo *info1){

    FILE *cr = fopen("/proc/stat", "r");

    if(!cr){
        return;
    }

    char store[256];
    fgets(store, sizeof(store), cr);

    unsigned long user, nice, system, idle;
    unsigned long iowait, irq, softirq, steal;

    sscanf(store, "cpu %lu %lu %lu %lu %lu %lu %lu %lu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);


    info1->user = user;
    info1->nice = nice;
    info1->system = system;
    info1->idle = idle;
    info1->iowait = iowait;
    info1->irq = irq;
    info1->softirq = softirq;
    info1->steal = steal;


    long Idle_initial = idle + iowait;
    long NonIdle_initial = user + nice + system + irq + softirq + steal;
    long Total_initial = Idle_initial + NonIdle_initial;

    usleep(100000);

    fseek(cr, 0, SEEK_SET);
    
    fgets(store, sizeof(store), cr);

    sscanf(store,"cpu %lu %lu %lu %lu %lu %lu %lu %lu",&user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);


    long Idle_final = idle + iowait;
    long NonIdle_final = user + nice + system + irq + softirq + steal;
    long Total_final = Idle_final + NonIdle_final;

    long dell_total = Total_final - Total_initial;

    long dell_idle = Idle_final - Idle_initial;

    double cpu_usage = ((double)(dell_total - dell_idle)/dell_total)*100;
    info1->cpu_usage = cpu_usage;


    fclose(cr);

}