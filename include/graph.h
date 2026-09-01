#ifndef GRAPH_H
#define GRAPH_H

#include <gtk/gtk.h>

#define GRAPH_HISTORY_LEN 60

typedef struct {
    double values[GRAPH_HISTORY_LEN];
    int head;
    double fixed_max; // 0.0 = autoscale to current max in buffer, >0 = fixed ceiling (e.g. 100.0 for percentages)
} GraphHistory;

void graph_history_init(GraphHistory *hist);
void graph_push_value(GraphHistory *hist, double value);
void graph_setup(GtkDrawingArea *area, GraphHistory *hist);

#endif