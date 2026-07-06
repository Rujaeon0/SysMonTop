#include "columnview.h"
#include "cpu.h"
#include "gio/gio.h"
#include "glib.h"
#include "process.h"
#include "process_object_gtk.h"
#include "unit_conversion.h"
#include <gtk/gtk.h>
#include <stdio.h>




typedef struct {

    GtkLabel *cpu_usage;
    GtkLabel *total_ram;
    GtkLabel *available_ram;
    GtkLabel *swap_total;
    GtkLabel *swap_free;
    GtkLabel *uptime;
    GtkColumnView *pid_view;

} AppWidgets;




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
    widgets->uptime = GTK_LABEL(gtk_builder_get_object(builder, "system_uptime_loading"));

    CPUInfo cpu_info;
    get_cpu_info(&cpu_info);
    char buffer[64];
    snprintf(buffer,sizeof(buffer),"%.2f %%",cpu_info.cpu_usage);
    gtk_label_set_text(widgets->cpu_usage, buffer);

    mem sys_mem;
    total_memory_read("/proc/meminfo", &sys_mem);
    char buffer_total_ram[32];
    char buffer_available_ram[32];
    char buffer_swap_total[32];
    char buffer_swap_free[32];
    snprintf(buffer_total_ram, sizeof(buffer_total_ram), "%.2Lf GiB",kib_to_gib(sys_mem.total_memory));
    snprintf(buffer_available_ram, sizeof(buffer_available_ram), "%.2Lf GiB",kib_to_gib(sys_mem.available_memory));
    snprintf(buffer_swap_total, sizeof(buffer_swap_total), "%.2Lf GiB",kib_to_gib(sys_mem.swap_total));
    snprintf(buffer_swap_free, sizeof(buffer_swap_free), "%.2Lf GiB",kib_to_gib(sys_mem.swap_free));
    gtk_label_set_text(widgets->total_ram, buffer_total_ram);
    gtk_label_set_text(widgets->available_ram, buffer_available_ram);
    gtk_label_set_text(widgets->swap_total, buffer_swap_total);
    gtk_label_set_text(widgets->swap_free, buffer_swap_free);

    
    
    long day=0,minute=0,hour=0,second=0;
    system_uptime(&day, &hour, &minute, &second);
    char buffer_time[64];
    snprintf(buffer_time, sizeof(buffer_time), "%ld days %ld hours %ld minutes %ld seconds",day,hour,minute,second);
    gtk_label_set_text(widgets->uptime, buffer_time);




    mem processes[1024];
    int count = process_scan(processes);
    GListStore *store = g_list_store_new(PROCESS_TYPE_OBJECT);
    for(int i = 0; i < count ; i++){
        ProcessObject *obj = process_object_new();
        obj->process = processes[i];
        g_list_store_append(store, obj);
        g_object_unref(obj);
        
    }
    
    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL(store));
    gtk_column_view_set_model(widgets->pid_view,GTK_SELECTION_MODEL(selection));
    setup_process_columnview(widgets->pid_view);




    

    
    gtk_window_present(GTK_WINDOW(window));

}




int main(int argc, char *argv[]){

    GtkApplication *app = gtk_application_new("com.gtk4apptest.SysMonTop", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(gtkcall), NULL);

    int status = g_application_run(G_APPLICATION(app),argc,argv);
    g_object_unref(app);
    return status;

}


