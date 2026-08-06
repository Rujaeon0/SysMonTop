#include "appwidgets.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "glib.h"
#include "gtk/gtksingleselection.h"
#include "process.h"
#include "process_object_gtk.h"
#include <gtk/gtk.h>
#include <stdlib.h>

gboolean column_refresh(gpointer data) {
  AppWidgets *widgets = data;
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

  return G_SOURCE_CONTINUE;
}
