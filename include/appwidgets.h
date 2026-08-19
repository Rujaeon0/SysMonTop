#ifndef APPWIDGETS_H
#define APPWIDGETS_H
#include <gtk/gtk.h>

typedef struct {

    GtkLabel *cpu_usage;
    GtkLabel *total_ram;
    GtkLabel *available_ram;
    GtkLabel *swap_total;
    GtkLabel *swap_free;
    GtkLabel *uptime;
    GListStore *store;
    GtkColumnView *pid_view;
    GtkSingleSelection *selection;
    GtkScrolledWindow *process_scroll;
    GHashTable *process_map;
} AppWidgets;


#endif