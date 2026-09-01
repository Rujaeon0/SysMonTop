#include "appwidgets.h"
#include "network.h"
#include "network_object_gtk.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "glib.h"
#include "gtk/gtkshortcut.h"
#include "gtk/gtksingleselection.h"
#include "process.h"
#include "process_object_gtk.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include "graph.h"


typedef struct {
  GtkAdjustment *vadj;
  double position;
} Scrolldata;

static gboolean restore_scroll(gpointer data){
  Scrolldata *scroll = data;
  gtk_adjustment_set_value(scroll->vadj, scroll->position);
  
  g_free(scroll);

  return G_SOURCE_REMOVE;

}

gboolean network_refresh(gpointer data){
    AppWidgets *widgets = data;

    NetworkIO network_info[20];
    int net_count = network_usage("/proc/net/dev",20,network_info);

    unsigned long long total_rate = 0;

    for(int i = 0; i < net_count; i++){

        NetworkObject *obj =
            g_list_model_get_item(G_LIST_MODEL(widgets->network_store),i);

        if(obj){
            network_object_set_network(obj,&network_info[i]);
            total_rate += network_object_get_rx_rate(obj)
                        + network_object_get_tx_rate(obj);
            g_object_unref(obj);
        }
    }

    graph_push_value(&widgets->network_history, (double)total_rate);
    gtk_widget_queue_draw(GTK_WIDGET(widgets->network_graph));

    return G_SOURCE_CONTINUE;
}
gboolean column_refresh(gpointer data) {
  AppWidgets *widgets = data;
  //GtkSingleSelection *selection = widgets->selection;
  GtkAdjustment *vadj = gtk_scrolled_window_get_vadjustment(widgets->process_scroll);
  double scroll_position = gtk_adjustment_get_value(vadj);
  GtkSingleSelection *selection = widgets->selection;
  guint selected_index = gtk_single_selection_get_selected(selection);
  long selected_pid = -1;
  if (selected_index != GTK_INVALID_LIST_POSITION) {
    ProcessObject *selected =
        g_list_model_get_item(G_LIST_MODEL(widgets->store), selected_index);

    if (selected) {
      selected_pid = selected->process.pid;
      g_object_unref(selected);
    }
  }

  g_list_store_remove_all(widgets->store);

  mem processes[1024];
  int count = process_scan(processes);
  qsort(processes, count, sizeof(mem), memory_sort);
  for (int i = 0; i < count; i++) {
    ProcessObject *obj = process_object_new();
    obj->process = processes[i];
    g_list_store_append(widgets->store, obj);
    g_object_unref(obj);
  }



  guint restore_index = GTK_INVALID_LIST_POSITION;

  for (guint i = 0; i < g_list_model_get_n_items(G_LIST_MODEL(widgets->store));
       i++) {

    ProcessObject *obj = g_list_model_get_item(G_LIST_MODEL(widgets->store), i);

    if (obj->process.pid == selected_pid) {
      restore_index = i;
      g_object_unref(obj);
      break;
    }

    g_object_unref(obj);
  }

  if (restore_index != GTK_INVALID_LIST_POSITION)
    gtk_single_selection_set_selected(selection, restore_index);

  Scrolldata *scroll = g_new(Scrolldata,1);
  scroll->vadj = vadj;
  scroll->position = scroll_position;

  g_idle_add(restore_scroll,scroll);


  return G_SOURCE_CONTINUE;
}
