#ifndef APPWIDGETS_H
#define APPWIDGETS_H
#include "graph.h"
#include "storage.h"
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
    GtkColumnView *network_view;
    GtkSingleSelection *selection;
    GtkScrolledWindow *process_scroll;
    GtkScrolledWindow *network;
    GListStore *network_store;
    GHashTable *process_map;
    GtkPaned *paned;
    GtkDrawingArea *storage_graph;
    GtkDrawingArea *memory_graph;
    GtkDrawingArea *network_graph;
    GraphHistory storage_history;
    GraphHistory memory_history;
    GraphHistory network_history;
    GtkDropDown *storage_selector;
    StorageInfo storage_devices[MAX_STORAGE_DEVICES];
    int storage_device_count;
    char selected_storage_path[STORAGE_PATH_LEN];


} AppWidgets;


#endif