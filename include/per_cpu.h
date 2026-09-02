#ifndef PID_CPU_H
#define PID_CPU_H

#include <glib.h>

typedef struct {
    long tracked_pid;
    unsigned long long prev_utime;
    unsigned long long prev_stime;
    gint64 prev_time;
    double cpu_percent;
} Tracker;

void tracker_init(Tracker *tracker);
double per_percentage(Tracker *tracker, long pid);

#endif