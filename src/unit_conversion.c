#include "include/unit_conversion.h"
#include <bits/time.h>
#include <unistd.h>
#include <time.h>

long double page_to_mib(long double pages){
    static long page_size = 0;

    if (page_size == 0){
        page_size = getpagesize();

    }
    return (pages*page_size)/(1024.0L*1024.0L); 
}

long double kib_to_gib(long double kib){

    return kib / (1024.0L *1024.0L);
}

long double mib_to_gib(long double mib){

    return mib/1024.0L;
}


long double kib_to_mib(long double kib){

    return kib/1024.0L;
}

void system_uptime(long *days, long *hours, long *minutes, long *seconds){

    
    struct timespec ts;

    if(clock_gettime(CLOCK_MONOTONIC, &ts)==0){
        long total_seconds = ts.tv_sec;
        *days = total_seconds/86400;
        *hours = (total_seconds%86400)/3600;
        *minutes = (total_seconds % 3600) / 60;
        *seconds = total_seconds % 60;



    }

    /* long double ticks_per_sec = sysconf(_SC_CLK_TCK);
    return ticks/ticks_per_sec; */
}
