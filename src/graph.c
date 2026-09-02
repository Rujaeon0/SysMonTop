#include "graph.h"
#include <stdio.h>

void graph_history_init(GraphHistory *hist){
    for (int i = 0; i < GRAPH_HISTORY_LEN; i++)
        hist->values[i] = 0.0;
    hist->head = 0;
    hist->fixed_max = 0.0;
    hist->hover_index = -1;
}

void graph_push_value(GraphHistory *hist, double value){
    hist->head = (hist->head + 1) % GRAPH_HISTORY_LEN;
    hist->values[hist->head] = value;
}

static void graph_draw_func(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data){
    GraphHistory *hist = user_data;

    cairo_set_source_rgba(cr, 0, 0, 0, 0.06);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    cairo_set_source_rgba(cr, 1, 1, 1, 0.08);
    cairo_set_line_width(cr, 1.0);
    for (int i = 1; i < 4; i++) {
        double y = height * i / 4.0;
        cairo_move_to(cr, 0, y);
        cairo_line_to(cr, width, y);
    }
    cairo_stroke(cr);

    double max_value = hist->fixed_max;
    if (max_value <= 0.0) {
        max_value = 1.0;
        for (int i = 0; i < GRAPH_HISTORY_LEN; i++)
            if (hist->values[i] > max_value)
                max_value = hist->values[i];
        max_value *= 1.15;
    }

    double points_x[GRAPH_HISTORY_LEN];
    double points_y[GRAPH_HISTORY_LEN];

    for (int i = 0; i < GRAPH_HISTORY_LEN; i++) {
        int idx = (hist->head + 1 + i) % GRAPH_HISTORY_LEN;
        double norm = hist->values[idx] / max_value;
        if (norm > 1.0) norm = 1.0;
        if (norm < 0.0) norm = 0.0;

        points_x[i] = (double)i / (GRAPH_HISTORY_LEN - 1) * width;
        points_y[i] = height - (norm * height);
    }

    cairo_move_to(cr, points_x[0], height);
    for (int i = 0; i < GRAPH_HISTORY_LEN; i++)
        cairo_line_to(cr, points_x[i], points_y[i]);
    cairo_line_to(cr, points_x[GRAPH_HISTORY_LEN - 1], height);
    cairo_close_path(cr);
    cairo_set_source_rgba(cr, 0.20, 0.55, 0.95, 0.25);
    cairo_fill(cr);

    cairo_move_to(cr, points_x[0], points_y[0]);
    for (int i = 1; i < GRAPH_HISTORY_LEN; i++)
        cairo_line_to(cr, points_x[i], points_y[i]);
    cairo_set_source_rgb(cr, 0.20, 0.55, 0.95);
    cairo_set_line_width(cr, 2.0);
    cairo_stroke(cr);

    char buf[32];
    if (hist->fixed_max > 0.0)
        snprintf(buf, sizeof(buf), "%.1f%%", hist->values[hist->head]);
    else
        snprintf(buf, sizeof(buf), "%.0f B/s", hist->values[hist->head]);

    cairo_set_source_rgba(cr, 1, 1, 1, 0.85);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 12);
    cairo_move_to(cr, 6, 14);
    cairo_show_text(cr, buf);

    if (hist->hover_index >= 0 && hist->hover_index < GRAPH_HISTORY_LEN){
        double hx = points_x[hist->hover_index];
        double hy = points_y[hist->hover_index];

        cairo_set_source_rgba(cr, 1, 1, 1, 0.25);
        cairo_set_line_width(cr, 1.0);
        cairo_move_to(cr, hx, 0);
        cairo_line_to(cr, hx, height);
        cairo_stroke(cr);

        cairo_set_source_rgb(cr, 0.20, 0.55, 0.95);
        cairo_arc(cr, hx, hy, 3.0, 0, 2 * G_PI);
        cairo_fill(cr);

        int idx = (hist->head + 1 + hist->hover_index) % GRAPH_HISTORY_LEN;
        char hover_buf[32];
        if (hist->fixed_max > 0.0)
            snprintf(hover_buf, sizeof(hover_buf), "%.1f%%", hist->values[idx]);
        else
            snprintf(hover_buf, sizeof(hover_buf), "%.0f B/s", hist->values[idx]);

        int seconds_ago = GRAPH_HISTORY_LEN - 1 - hist->hover_index;
        char label_buf[48];
        snprintf(label_buf, sizeof(label_buf), "%s (%ds ago)", hover_buf, seconds_ago);

        cairo_set_source_rgba(cr, 1, 1, 1, 0.9);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 11);
        cairo_move_to(cr, hx + 6, height - 8);
        cairo_show_text(cr, label_buf);
    }
}

static void graph_motion_cb(GtkEventControllerMotion *controller, double x, double y, gpointer user_data){
    GraphHistory *hist = user_data;

    GtkWidget *area = gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(controller));
    int width = gtk_widget_get_width(area);

    if (width <= 0)
        return;

    int index = (int)((x / width) * (GRAPH_HISTORY_LEN - 1));
    if (index < 0) index = 0;
    if (index >= GRAPH_HISTORY_LEN) index = GRAPH_HISTORY_LEN - 1;

    hist->hover_index = index;
    gtk_widget_queue_draw(area);
}

static void graph_leave_cb(GtkEventControllerMotion *controller, gpointer user_data){
    GraphHistory *hist = user_data;
    hist->hover_index = -1;

    GtkWidget *area = gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(controller));
    gtk_widget_queue_draw(area);
}

void graph_setup(GtkDrawingArea *area, GraphHistory *hist){
    gtk_drawing_area_set_draw_func(area, graph_draw_func, hist, NULL);

    GtkEventController *motion = gtk_event_controller_motion_new();

    g_signal_connect(motion, "motion", G_CALLBACK(graph_motion_cb), hist);
    g_signal_connect(motion, "leave", G_CALLBACK(graph_leave_cb), hist);

    gtk_widget_add_controller(GTK_WIDGET(area), motion);
}