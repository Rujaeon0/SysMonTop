#include "graph.h"
#include <stdio.h>

void graph_history_init(GraphHistory *hist){
    for (int i = 0; i < GRAPH_HISTORY_LEN; i++)
        hist->values[i] = 0.0;
    hist->head = 0;
    hist->fixed_max = 0.0;
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
}

void graph_setup(GtkDrawingArea *area, GraphHistory *hist){
    gtk_drawing_area_set_draw_func(area, graph_draw_func, hist, NULL);
}