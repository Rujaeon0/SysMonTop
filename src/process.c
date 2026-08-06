#include "include/process.h"
#include "unit_conversion.h"
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>



void get_mem_info(const char *path,mem *info2){

    FILE *mi = fopen(path, "r"); //Memory info/path-->>/proc/"path"/statm
    
    if(!mi){
        printf("Error from mi");
        return;
    }
    
    
    
    char storem[256];
    fgets(storem, sizeof(storem), mi);
    
    double long memused;
    double long virt_total;
    
    sscanf(storem, "%Lf %Lf",&virt_total,&memused);
    
    //info2->memfree = memfree;
    info2->memused = memused;
    
    fclose(mi);

    
}

void total_memory_read(const char *path_meminfo, mem *info2){
    char memtotal[256];
    FILE *totals = fopen(path_meminfo, "r");

    if(!totals){
        printf("Error from totals");
        return;
    }
    
    int found = 0;
    while(fgets(memtotal, sizeof(memtotal), totals)){
        if(sscanf(memtotal, "MemTotal: %Lf kB", &info2->total_memory) == 1)
            found++;
    
        else if(sscanf(memtotal, "MemAvailable: %Lf kB",&info2->available_memory) == 1)
            found++;

        else if(sscanf(memtotal,"SwapTotal: %Lf kB", &info2->swap_total)==1)
            found++;

        else if(sscanf(memtotal,"SwapFree: %Lf kB", &info2->swap_free)==1)
            found++;

        if (found == 4)
            break;

    }

    fclose(totals);

  
}



void pid_name(const char *path,mem *info2){

    FILE *pn = fopen(path, "r");  //pn=process_name

    if(!pn){
        printf("Error");
        return;
    }
    char storen[256];
    fgets(storen, sizeof(storen),pn);

    sscanf(storen, "Name:%255s", info2->processname);

    fclose(pn);

}





//process_scan contains my scanning processes like scans the whole /proc directory for numerics only then takes in then print out the info using other functions

int process_scan(mem processes[]){
    DIR *directory;
    struct dirent *entry;
    directory = opendir("/proc");
    mem info2;
    int c = 0;

    if(directory == NULL){
        printf("Error opening dir\n");
        return 1;
    }
//Till here it opens the /proc using opendir
    
    while ((entry = readdir(directory)) != NULL){
        
        char *endptr;
        long pid = strtol(entry->d_name, &endptr, 10);
        if (endptr == entry->d_name ||*endptr != '\0') {

            continue;
        }
        //Till here it opens all the PID files or only files that contains numeric value
                                                               
        char statm_path[256];
        char status[256];

        snprintf(statm_path, sizeof(statm_path), "/proc/%ld/statm",pid);
        snprintf(status, sizeof(status), "/proc/%ld/status", pid);
        pid_name(status,&info2);
        get_mem_info(statm_path, &info2);
        if(page_to_mib(info2.memused) < 1.0)
            continue;
        if (info2.processname[0] == '[')
            continue;
        processes[c] = info2;
        processes[c].pid = pid;
        c++;
        

    }
    
    if(closedir(directory) == -1){
        printf("Error closing dir\n");
        return 1;
    }
    return c;

}


int memory_sort(const void *a, const void *b){

    const mem *p1 = (const mem *)a;
    const mem *p2 = (const mem *)b;
    if(p1->memused < p2->memused)
        return 1;
    if(p1->memused > p2->memused)
        return -1;

    return 0;

}


