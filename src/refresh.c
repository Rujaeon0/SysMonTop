#include "refresh.h"
#include "appwidgets.h"
#include "cpu.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "network.h"
#include "network_object_gtk.h"
#include "glib.h"
#include "process.h"
#include "unit_conversion.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include "storage.h"
#include "graph.h"


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

    double mem_percent = 0.0;
    if (sys_mem.total_memory > 0)
        mem_percent = ((double)(sys_mem.total_memory - sys_mem.available_memory)
                        / (double)sys_mem.total_memory) * 100.0;
    graph_push_value(&widgets->memory_history, mem_percent);
    gtk_widget_queue_draw(GTK_WIDGET(widgets->memory_graph));

    double storage_percent = get_storage_usage_percentage(widgets->selected_storage_path);
    graph_push_value(&widgets->storage_history, storage_percent);
    gtk_widget_queue_draw(GTK_WIDGET(widgets->storage_graph));

    long d,h,m,s;
    system_uptime(&d, &h, &m, &s);
    snprintf(buffer_full, sizeof(buffer_full),"%ld Days %ld Hours %ld Minutes %ld Seconds",d,h,m,s);
    gtk_label_set_text(widgets->uptime, buffer_full);

    NetworkIO network_info[20];
    int net_count = network_usage("/proc/net/dev",20,network_info);
    guint network_items = g_list_model_get_n_items(G_LIST_MODEL(widgets->network_store));
    for(guint i=0; i < network_items && i < (guint)net_count; i++){
        NetworkObject *obj = g_list_model_get_item(G_LIST_MODEL(widgets->network_store),i);
        if(obj){
            network_object_set_network(obj,&network_info[i]);
            g_object_unref(obj);
        }
    }

    return G_SOURCE_CONTINUE;
}