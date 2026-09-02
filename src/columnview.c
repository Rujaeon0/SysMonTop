#include "columnview.h"
#include "glib-object.h"
#include "glib.h"
#include "gtk/gtk.h"
#include "gtk/gtkshortcut.h"
#include "process.h"
#include "process_object_gtk.h"
#include "unit_conversion.h"
#include "network_object_gtk.h"
#include <stdio.h>
#include <unistd.h>


static void pid_setup(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label), 0.5);
    gtk_list_item_set_child(list_item, label);
}

static void pid_bind(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    ProcessObject *obj = PROCESS_OBJECT(gtk_list_item_get_item(list_item));
    GtkLabel *label = GTK_LABEL(gtk_list_item_get_child(list_item));

    char buffer_pid[32];

    snprintf(buffer_pid,sizeof(buffer_pid),"%ld",obj->process.pid);
    gtk_label_set_text(label,buffer_pid);

    if (obj->process.pid == (long)getpid())
        gtk_widget_add_css_class(GTK_WIDGET(label), "own-process");
    else
        gtk_widget_remove_css_class(GTK_WIDGET(label), "own-process");
}


static void name_setup(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label), 0.5);
    gtk_list_item_set_child(list_item,label);
}

static void name_bind(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    ProcessObject *obj = PROCESS_OBJECT(gtk_list_item_get_item(list_item));
    GtkLabel *label = GTK_LABEL(gtk_list_item_get_child(list_item));

    gtk_label_set_text(label,obj->process.processname);

    if (obj->process.pid == (long)getpid())
        gtk_widget_add_css_class(GTK_WIDGET(label), "own-process");
    else
        gtk_widget_remove_css_class(GTK_WIDGET(label), "own-process");
}


static void memory_setup(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label), 0.5);
    gtk_list_item_set_child(list_item,label);
}

static void memory_bind(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    ProcessObject *obj = PROCESS_OBJECT(gtk_list_item_get_item(list_item));
    GtkLabel *label = GTK_LABEL(gtk_list_item_get_child(list_item));

    char buffer_mem[64];

    snprintf(buffer_mem,sizeof(buffer_mem),"%.2Lf MiB",page_to_mib(obj->process.memused));
    gtk_label_set_text(label,buffer_mem);

    if (obj->process.pid == (long)getpid())
        gtk_widget_add_css_class(GTK_WIDGET(label), "own-process");
    else
        gtk_widget_remove_css_class(GTK_WIDGET(label), "own-process");
}


static int compare_pid(gconstpointer a, gconstpointer b, gpointer user_data){
    const ProcessObject *pa = PROCESS_OBJECT((gpointer)a);
    const ProcessObject *pb = PROCESS_OBJECT((gpointer)b);
    return (pa->process.pid > pb->process.pid) - (pa->process.pid < pb->process.pid);
}

static int compare_name(gconstpointer a, gconstpointer b, gpointer user_data){
    const ProcessObject *pa = PROCESS_OBJECT((gpointer)a);
    const ProcessObject *pb = PROCESS_OBJECT((gpointer)b);
    return g_strcmp0(pa->process.processname, pb->process.processname);
}

static int compare_memory(gconstpointer a, gconstpointer b, gpointer user_data){
    const ProcessObject *pa = PROCESS_OBJECT((gpointer)a);
    const ProcessObject *pb = PROCESS_OBJECT((gpointer)b);
    if (pa->process.memused > pb->process.memused) return -1;
    if (pa->process.memused < pb->process.memused) return 1;
    return 0;
}


void setup_process_columnview(GtkColumnView *view){

    GtkListItemFactory *pid_factory = gtk_signal_list_item_factory_new();

    g_signal_connect(pid_factory,"setup",G_CALLBACK(pid_setup),NULL);
    g_signal_connect(pid_factory,"bind",G_CALLBACK(pid_bind),NULL);

    GtkColumnViewColumn *pid_column = gtk_column_view_column_new("PID",pid_factory);

    gtk_column_view_column_set_fixed_width(pid_column,80);
    gtk_column_view_append_column(view,pid_column);

    GtkSorter *pid_sorter = GTK_SORTER(gtk_custom_sorter_new((GCompareDataFunc)compare_pid, NULL, NULL));
    gtk_column_view_column_set_sorter(pid_column, pid_sorter);
    g_object_unref(pid_sorter);


    GtkListItemFactory *name_factory = gtk_signal_list_item_factory_new();

    g_signal_connect(name_factory,"setup",G_CALLBACK(name_setup),NULL);
    g_signal_connect(name_factory,"bind",G_CALLBACK(name_bind),NULL);

    GtkColumnViewColumn *name_column = gtk_column_view_column_new("Name",name_factory);

    gtk_column_view_column_set_fixed_width(name_column,220);
    gtk_column_view_append_column(view,name_column);

    GtkSorter *name_sorter = GTK_SORTER(gtk_custom_sorter_new((GCompareDataFunc)compare_name, NULL, NULL));
    gtk_column_view_column_set_sorter(name_column, name_sorter);
    g_object_unref(name_sorter);


    GtkListItemFactory *memory_factory = gtk_signal_list_item_factory_new();

    g_signal_connect(memory_factory,"setup",G_CALLBACK(memory_setup),NULL);
    g_signal_connect(memory_factory,"bind",G_CALLBACK(memory_bind),NULL);

    GtkColumnViewColumn *memory_column = gtk_column_view_column_new("Memory",memory_factory);

    gtk_column_view_column_set_fixed_width(memory_column,120);
    gtk_column_view_append_column(view,memory_column);

    GtkSorter *memory_sorter = GTK_SORTER(gtk_custom_sorter_new((GCompareDataFunc)compare_memory, NULL, NULL));
    gtk_column_view_column_set_sorter(memory_column, memory_sorter);
    g_object_unref(memory_sorter);
}


static void interface_setup(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    GtkWidget *label = gtk_label_new(NULL);
    gtk_list_item_set_child(list_item,label);
}

static void interface_bind(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    NetworkObject *obj = NETWORK_OBJECT(gtk_list_item_get_item(list_item));
    GtkLabel *label = GTK_LABEL(gtk_list_item_get_child(list_item));

    const NetworkIO *network = network_object_get_network(obj);

    gtk_label_set_text(label,network->interface0);
}


static void rx_rate_setup(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label),0.5);
    gtk_list_item_set_child(list_item,label);
}

static void rx_rate_updated(NetworkObject *obj,gpointer data){
    GtkLabel *label = GTK_LABEL(data);

    unsigned long long rx_rate = network_object_get_rx_rate(obj);

    char buffer_rx[64];

    snprintf(buffer_rx,sizeof(buffer_rx),"%llu B/s",rx_rate);
    gtk_label_set_text(label,buffer_rx);
}

static void rx_rate_bind(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    NetworkObject *obj = NETWORK_OBJECT(gtk_list_item_get_item(list_item));
    GtkLabel *label = GTK_LABEL(gtk_list_item_get_child(list_item));

    rx_rate_updated(obj,label);

    g_signal_connect(obj,"updated",G_CALLBACK(rx_rate_updated),label);
}

static void rx_rate_unbind(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    NetworkObject *obj = NETWORK_OBJECT(gtk_list_item_get_item(list_item));
    GtkLabel *label = GTK_LABEL(gtk_list_item_get_child(list_item));

    if(obj){
        g_signal_handlers_disconnect_by_func(
            obj,
            G_CALLBACK(rx_rate_updated),
            label
        );
    }
}


static void tx_rate_setup(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label),0.5);
    gtk_list_item_set_child(list_item,label);
}

static void tx_rate_updated(NetworkObject *obj,gpointer data){
    GtkLabel *label = GTK_LABEL(data);

    unsigned long long tx_rate = network_object_get_tx_rate(obj);

    char buffer_tx[64];

    snprintf(buffer_tx,sizeof(buffer_tx),"%llu B/s",tx_rate);
    gtk_label_set_text(label,buffer_tx);
}

static void tx_rate_bind(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    NetworkObject *obj = NETWORK_OBJECT(gtk_list_item_get_item(list_item));
    GtkLabel *label = GTK_LABEL(gtk_list_item_get_child(list_item));

    tx_rate_updated(obj,label);

    g_signal_connect(obj,"updated",G_CALLBACK(tx_rate_updated),label);
}

static void tx_rate_unbind(GtkSignalListItemFactory *factory,GtkListItem *list_item,gpointer data){
    NetworkObject *obj = NETWORK_OBJECT(gtk_list_item_get_item(list_item));
    GtkLabel *label = GTK_LABEL(gtk_list_item_get_child(list_item));

    if(obj){
        g_signal_handlers_disconnect_by_func(
            obj,
            G_CALLBACK(tx_rate_updated),
            label
        );
    }
}


void setup_network_columnview(GtkColumnView *view){

    GtkListItemFactory *interface_factory = gtk_signal_list_item_factory_new();

    g_signal_connect(interface_factory,"setup",G_CALLBACK(interface_setup),NULL);
    g_signal_connect(interface_factory,"bind",G_CALLBACK(interface_bind),NULL);

    GtkColumnViewColumn *interface_column =
        gtk_column_view_column_new("Interface",interface_factory);

    gtk_column_view_column_set_expand(interface_column, TRUE);
    gtk_column_view_append_column(view,interface_column);


    GtkListItemFactory *rx_rate_factory = gtk_signal_list_item_factory_new();

    g_signal_connect(rx_rate_factory,"setup",G_CALLBACK(rx_rate_setup),NULL);
    g_signal_connect(rx_rate_factory,"bind",G_CALLBACK(rx_rate_bind),NULL);
    g_signal_connect(rx_rate_factory,"unbind",G_CALLBACK(rx_rate_unbind),NULL);

    GtkColumnViewColumn *rx_rate_column =
        gtk_column_view_column_new("RX Rate",rx_rate_factory);

    gtk_column_view_column_set_expand(rx_rate_column, TRUE);
    gtk_column_view_append_column(view,rx_rate_column);


    GtkListItemFactory *tx_rate_factory = gtk_signal_list_item_factory_new();

    g_signal_connect(tx_rate_factory,"setup",G_CALLBACK(tx_rate_setup),NULL);
    g_signal_connect(tx_rate_factory,"bind",G_CALLBACK(tx_rate_bind),NULL);
    g_signal_connect(tx_rate_factory,"unbind",G_CALLBACK(tx_rate_unbind),NULL);

    GtkColumnViewColumn *tx_rate_column =
        gtk_column_view_column_new("TX Rate",tx_rate_factory);

    gtk_column_view_column_set_expand(tx_rate_column, TRUE);
    gtk_column_view_append_column(view,tx_rate_column);
}