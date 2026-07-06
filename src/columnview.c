#include "columnview.h"
#include "process.h"
#include "process_object_gtk.h"
#include "unit_conversion.h"
#include <stdio.h>

static void pid_setup(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    GtkWidget *label = gtk_label_new(NULL);
    gtk_list_item_set_child(list_item, label);
}
static void pid_bind(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer data){
    ProcessObject *obj = PROCESS_OBJECT(gtk_list_item_get_item(list_item));
    GtkLabel *label = GTK_LABEL(gtk_list_item_get_child(list_item));
    char buffer_pid[32];
    snprintf(buffer_pid, sizeof(buffer_pid), "%ld", obj->process.pid);
    gtk_label_set_text(label, buffer_pid);
    
}




static void name_setup(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    GtkWidget *label = gtk_label_new(NULL);
    gtk_list_item_set_child(list_item, label);
}
static void
name_bind(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data)
{
    ProcessObject *obj = PROCESS_OBJECT(gtk_list_item_get_item(list_item));

    GtkLabel *label = GTK_LABEL(gtk_list_item_get_child(list_item));

    gtk_label_set_text(label,obj->process.processname);
}



static void memory_setup(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    GtkWidget *label = gtk_label_new(NULL);
    gtk_list_item_set_child(list_item, label);
}
static void memory_bind(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer data){
    ProcessObject *obj = PROCESS_OBJECT(gtk_list_item_get_item(list_item));

    GtkLabel *label = GTK_LABEL(gtk_list_item_get_child(list_item));
    
    char buffer_mem[64];
    snprintf(buffer_mem,sizeof(buffer_mem),"%.2Lf MiB",page_to_mib(obj->process.memused));
    gtk_label_set_text(label,buffer_mem);
}




void setup_process_columnview(GtkColumnView *view){

    GtkListItemFactory *pid_factory = gtk_signal_list_item_factory_new();
    g_signal_connect(pid_factory, "setup", G_CALLBACK(pid_setup),NULL);
    g_signal_connect(pid_factory, "bind", G_CALLBACK(pid_bind),NULL);
    GtkColumnViewColumn *pid_column = gtk_column_view_column_new("PID", pid_factory);
    gtk_column_view_append_column(view, pid_column);

    GtkListItemFactory *name_factory = gtk_signal_list_item_factory_new();
    g_signal_connect(name_factory, "setup", G_CALLBACK(name_setup),NULL);
    g_signal_connect(name_factory, "bind", G_CALLBACK(name_bind),NULL);
    GtkColumnViewColumn *name_column = gtk_column_view_column_new("Name", name_factory);
    gtk_column_view_append_column(view, name_column);

    GtkListItemFactory *memory_factory = gtk_signal_list_item_factory_new();
    g_signal_connect(memory_factory, "setup", G_CALLBACK(memory_setup),NULL);
    g_signal_connect(memory_factory, "bind", G_CALLBACK(memory_bind),NULL);
    GtkColumnViewColumn *memory_column = gtk_column_view_column_new("Memory", memory_factory);
    gtk_column_view_append_column(view, memory_column);
    
}