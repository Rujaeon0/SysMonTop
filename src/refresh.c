#include "refresh.h"
#include "appwidgets.h"
#include "cpu.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "glib.h"
#include "gtk/gtksingleselection.h"
#include "process.h"
#include "process_object_gtk.h"
#include "unit_conversion.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

gboolean refresh_ui(gpointer data){
    AppWidgets *widgets = data;

    CPUInfo cpu_info;
    get_cpu_info(&cpu_info);

    char buffer_full[64];
    snprintf(buffer_full,sizeof(buffer_full),"%.2f %%",cpu_info.cpu_usage);
    gtk_label_set_text(widgets->cpu_usage,buffer_full);

    mem sys_mem;
    total_memory_read("/proc/meminfo", &sys_mem);
    snprintf(buffer_full,sizeof(buffer_full),"%.2Lf GiB",kib_to_gib(sys_mem.total_memory));
    gtk_label_set_text(widgets->total_ram, buffer_full);
    snprintf(buffer_full,sizeof(buffer_full),"%.2Lf GiB",kib_to_gib(sys_mem.available_memory));
    gtk_label_set_text(widgets->available_ram, buffer_full);
    snprintf(buffer_full,sizeof(buffer_full),"%.2Lf GiB",kib_to_gib(sys_mem.swap_total));
    gtk_label_set_text(widgets->swap_total, buffer_full);
    snprintf(buffer_full,sizeof(buffer_full),"%.2Lf GiB",kib_to_gib(sys_mem.swap_free));
    gtk_label_set_text(widgets->swap_free, buffer_full);

    long d,h,m,s;
    system_uptime(&d, &h, &m, &s);
    snprintf(buffer_full, sizeof(buffer_full),"%ld Days %ld Hours %ld Minutes %ld Seconds",d,h,m,s);
    gtk_label_set_text(widgets->uptime, buffer_full);


GtkSingleSelection *selection = widgets->selection;
guint selected_index = gtk_single_selection_get_selected(selection);
long selected_pid = -1;
if (selected_index != GTK_INVALID_LIST_POSITION) {
    ProcessObject *selected = g_list_model_get_item(G_LIST_MODEL(widgets->store), selected_index);

    if (selected) {
        selected_pid = selected->process.pid;
        g_object_unref(selected);
    }
}

    g_list_store_remove_all(widgets->store);

    mem processes[1024];
    int count = process_scan(processes);
    qsort(processes,count,sizeof(mem),memory_sort);
    for(int i = 0; i < count; i++){
        ProcessObject *obj = process_object_new();
        obj->process = processes[i];
        g_list_store_append(widgets->store,obj);
        g_object_unref(obj);
    }

guint restore_index = GTK_INVALID_LIST_POSITION;

for (guint i = 0; i < g_list_model_get_n_items(G_LIST_MODEL(widgets->store)); i++) {

    ProcessObject *obj =
        g_list_model_get_item(G_LIST_MODEL(widgets->store), i);

    if (obj->process.pid == selected_pid) {
        restore_index = i;
        g_object_unref(obj);
        break;
    }

    g_object_unref(obj);
}

if (restore_index != GTK_INVALID_LIST_POSITION)
    gtk_single_selection_set_selected(selection, restore_index);



return G_SOURCE_CONTINUE;

}



