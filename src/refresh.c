#include "refresh.h"
#include "appwidgets.h"
#include "cpu.h"
#include "glib.h"
#include "process.h"
#include "unit_conversion.h"
#include <gtk/gtk.h>
#include <stdio.h>

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


return G_SOURCE_CONTINUE;

}



