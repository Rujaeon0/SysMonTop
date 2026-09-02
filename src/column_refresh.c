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

    for(int i = 0; i < net_count; i++){

        NetworkObject *obj =
            g_list_model_get_item(G_LIST_MODEL(widgets->network_store),i);

        if(obj){
            network_object_set_network(obj,&network_info[i]);
            g_object_unref(obj);
        }
    }

    return G_SOURCE_CONTINUE;
}

gboolean column_refresh(gpointer data) {
  AppWidgets *widgets = data;
  GtkAdjustment *vadj = gtk_scrolled_window_get_vadjustment(widgets->process_scroll);
  double scroll_position = gtk_adjustment_get_value(vadj);
  GtkSingleSelection *selection = widgets->selection;
  guint selected_index = gtk_single_selection_get_selected(selection);
  long selected_pid = -1;
  if (selected_index != GTK_INVALID_LIST_POSITION) {
    ProcessObject *selected =
        g_list_model_get_item(G_LIST_MODEL(widgets->selection), selected_index);

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

  for (guint i = 0; i < g_list_model_get_n_items(G_LIST_MODEL(widgets->selection));
       i++) {

    ProcessObject *obj = g_list_model_get_item(G_LIST_MODEL(widgets->selection), i);

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