#include "columnview.h"
#include "column_refresh.h"
#include "gio/gio.h"
#include "glib.h"
#include "glibconfig.h"
#include "gtk/gtksingleselection.h"
#include "network.h"
#include "network_object_gtk.h"
#include "process.h"
#include "process_object_gtk.h"
#include "refresh.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <appwidgets.h>


static void lock_paned_position(GtkPaned *paned, GParamSpec *pspec, gpointer data){
    gtk_paned_set_position(paned,805);
}

static void gtkcall(GtkApplication *app, gpointer data){

    GtkBuilder *builder = gtk_builder_new_from_file("ui/SysMonTop.ui");

    GtkApplicationWindow *window = GTK_APPLICATION_WINDOW(gtk_builder_get_object(builder, "main_window"));

    gtk_window_set_application(GTK_WINDOW(window), app);

    AppWidgets *widgets = g_new0(AppWidgets, 1);


    widgets->cpu_usage = GTK_LABEL(gtk_builder_get_object(builder, "cpu_usage_loading"));
    widgets->total_ram = GTK_LABEL(gtk_builder_get_object(builder, "total_ram_loading"));
    widgets->available_ram = GTK_LABEL(gtk_builder_get_object(builder, "availble_ram_loading"));
    widgets->swap_total = GTK_LABEL(gtk_builder_get_object(builder, "virt_mem_loading"));
    widgets->swap_free = GTK_LABEL(gtk_builder_get_object(builder, "avai_virt_mem_loading"));
    widgets->pid_view = GTK_COLUMN_VIEW(gtk_builder_get_object(builder, "pid_view_loading"));
    widgets->process_scroll = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "process_scroll"));
    widgets->network_view = GTK_COLUMN_VIEW(gtk_builder_get_object(builder, "network_view"));
    widgets->network = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "network"));
    widgets->network_store = g_list_store_new(NETWORK_TYPE_OBJECT);
    widgets->uptime = GTK_LABEL(gtk_builder_get_object(builder, "system_uptime_loading"));
    widgets->paned = GTK_PANED(gtk_builder_get_object(builder, "columns"));
    gtk_paned_set_position(widgets->paned,475);
    gtk_widget_set_hexpand(GTK_WIDGET(widgets->paned),TRUE);
    g_signal_connect(widgets->paned,"notify::position",G_CALLBACK(lock_paned_position),NULL);



    mem processes[1024];
    int count = process_scan(processes);
    qsort(processes,count,sizeof(mem),memory_sort);
    widgets->store = g_list_store_new(PROCESS_TYPE_OBJECT);
    widgets->process_map = g_hash_table_new(g_direct_hash, g_direct_equal);
    for(int i = 0; i < count ; i++){
        ProcessObject *obj = process_object_new();
        obj->process = processes[i];
        g_list_store_append(widgets->store, obj);
        g_hash_table_insert(widgets->process_map,GINT_TO_POINTER((gint)processes[i].pid),g_object_ref(obj));
        g_object_unref(obj);
        
    }
    
    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL(widgets->store));
    widgets->selection = selection;
    gtk_column_view_set_model(widgets->pid_view,GTK_SELECTION_MODEL(selection));
    setup_process_columnview(widgets->pid_view);



    NetworkIO network_info[20];
    int net_count = network_usage("/proc/net/dev", 20, network_info);

    for(int i=0;i<net_count;i++){
        NetworkObject *obj = network_object_new();
        network_object_set_network(obj, &network_info[i]);
        g_list_store_append(widgets->network_store,obj);
        g_object_unref(obj);
    }
    GtkSingleSelection *network_selection = gtk_single_selection_new(G_LIST_MODEL(widgets->network_store));
    gtk_column_view_set_model(widgets->network_view, GTK_SELECTION_MODEL(network_selection));
    setup_network_columnview(widgets->network_view);



    g_timeout_add(1000, refresh_ui, widgets);
    g_timeout_add(3000, column_refresh, widgets);
    g_timeout_add(1000, network_refresh, widgets);



    gtk_window_present(GTK_WINDOW(window));

}




int main(int argc, char *argv[]){

    GtkApplication *app = gtk_application_new("com.gtk4apptest.SysMonTop", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(gtkcall), NULL);

    int status = g_application_run(G_APPLICATION(app),argc,argv);
    g_object_unref(app);
    return status;

}


