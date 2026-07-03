#include "cpu.h"
#include "include/process.h"
#include "unit_conversion.h"
#include <stdio.h>

int main(void){

    mem sys_mem;

    
    total_memory_read("/proc/meminfo", &sys_mem);
    printf("Total RAM: %Lf GiB\n", kib_to_gib(sys_mem.total_memory));
    printf("Available RAM: %Lf GiB\n", kib_to_gib(sys_mem.available_memory));
    printf("Total Swap: %Lf GiB\n", kib_to_gib(sys_mem.swap_total));
    printf("Free Swap: %Lf GiB\n", kib_to_gib(sys_mem.swap_free));
    

    CPUInfo cpu;


    get_cpu_info(&cpu);
    long d = 0, h = 0, m = 0, s = 0;

    system_uptime(&d,&h,&m,&s);
    
    printf("Uptime: %ld days, %ld hours, %ld minutes %ld seconds\n",d,h,m,s);
    printf("CPU Usage: %f\n",cpu.cpu_usage);

    mem processes[4096];
    int c = process_scan(processes);

    if(c < 0) {
        printf("Failed to scan for process...\n");
        return 69;
    }

    for(int i = 0; i < c; i++){
        printf("PID: %ld\nName: %s\nMemory: %Lf MiB\n",processes[i].pid,processes[i].processname,page_to_mib(processes[i].memused));
    }
}


