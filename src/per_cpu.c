#include "per_cpu.h"
#include "glib.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void tracker_init(Tracker *tracker){
    tracker->tracked_pid = -1;
    tracker->prev_utime = 0;
    tracker->prev_stime = 0;
    tracker->prev_time = 0;
    tracker->cpu_percent = 0.0;

}

static int read_pid_ticks(long pid, unsigned long long *utime, unsigned long long *stime){
    char path[64];
    snprintf(path,sizeof(path),"/proc/%ld/stat",pid);
    
    FILE *f = fopen(path, "r");
    if(!f)
        return -1;

    char buffer[512];
    if(!fgets(buffer,sizeof(buffer),f)){
        fclose(f);
        return -1;
    }
    fclose(f);
    char *after_comm = strrchr(buffer, ')');
    if(!after_comm)
        return -1;

    char state;
    long ppid, pgrp, session, tty_nr, tpgid;
    unsigned long flags, minflt, cminflt, majflt, cmajflt;
    unsigned long long u, s;

    int matched = sscanf(after_comm + 2,"%c %ld %ld %ld %ld %ld %lu %lu %lu %lu %lu %llu %llu",&state, &ppid, &pgrp, &session, &tty_nr, &tpgid,&flags, &minflt, &cminflt, &majflt, &cmajflt,&u, &s);
    if(matched != 13)
        return -1;
        
    *utime = u;
    *stime = s;
    return 0;

}

double per_percentage(Tracker *tracker, long pid){
    unsigned long long utime, stime;

    if(read_pid_ticks(pid, &utime, &stime) != 0)
        return -1.0;

    gint64 current_time = g_get_monotonic_time();

        if (tracker->tracked_pid != pid){
        tracker->tracked_pid = pid;
        tracker->prev_utime = utime;
        tracker->prev_stime = stime;
        tracker->prev_time = current_time;
        tracker->cpu_percent = 0.0;
        return 0.0;
    }
    gint64 elapsed_time = current_time - tracker->prev_time;

    // guard against div-by-zero / noisy readings on very rapid re-calls
    if (elapsed_time < 100000) // under 100ms
        return tracker->cpu_percent;

    long clock_ticks_per_sec = sysconf(_SC_CLK_TCK);
    unsigned long long delta_ticks =
        (utime - tracker->prev_utime) + (stime - tracker->prev_stime);

    double elapsed_seconds = elapsed_time / 1000000.0;
    double cpu_percent =
        ((double)delta_ticks / (double)clock_ticks_per_sec) / elapsed_seconds * 100.0;

    tracker->prev_utime = utime;
    tracker->prev_stime = stime;
    tracker->prev_time = current_time;
    tracker->cpu_percent = cpu_percent;

    return cpu_percent;
}