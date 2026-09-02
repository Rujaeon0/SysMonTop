#ifndef APPWIDGETS_H
#define APPWIDGETS_H
#include "graph.h"
#include "per_cpu.h"
#include "storage.h"
#include <gtk/gtk.h>

typedef struct {

    GtkLabel *cpu_usage;
    GtkLabel *total_ram;
    GtkLabel *available_ram;
    GtkLabel *swap_total;
    GtkLabel *swap_free;
    GtkLabel *overview_uptime_label;
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
    GtkPaned *inner_paned;
    GtkStack *detail_stack;
    GtkLabel *detail_pid_label;
    GtkLabel *detail_name_label;
    GtkLabel *detail_ram_label;
    GtkLabel *detail_cpu_label;
    GtkLabel *detail_status_label;
    GtkButton *kill_button;
    GtkButton *force_kill_button;
    GtkLevelBar *cpu_usage_bar;
    GtkLevelBar *memory_usage_bar;
    GtkLevelBar *swap_usage_bar;
    long selected_detail_cpu;
    Tracker detail_cpu_tracker;

} AppWidgets;


#endif