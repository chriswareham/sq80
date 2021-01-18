/*
 * Copyright (c) 2021 Chris Wareham <chris@chriswareham.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <gtk/gtk.h>

#include "main.h"
#include "dialog.h"
#include "filter.h"

static ScaleParams resonance_params = {
    .parameter = 0x6f,
    .multiplier = 4
};

static ScaleParams keyboard_tracking_params = {
    .parameter = 0x70,
    .multiplier = 2
};

static ComboBoxEntry mod1_srcs[] = {
    { "LFO 1",               0x71, 0x00 },
    { "LFO 2",               0x71, 0x08 },
    { "LFO 3",               0x71, 0x10 },
    { "Envelope 1",          0x71, 0x18 },
    { "Envelope 2",          0x71, 0x20 },
    { "Envelope 3",          0x71, 0x28 },
    { "Envelope 4",          0x71, 0x30 },
    { "Velocity",            0x71, 0x38 },
    { "Velocity X",          0x71, 0x40 },
    { "Keyboard",            0x71, 0x48 },
    { "Keyboard 2",          0x71, 0x50 },
    { "Modulation Wheel",    0x71, 0x58 },
    { "Foot Pedal",          0x71, 0x60 },
    { "External Controller", 0x71, 0x68 },
    { "Pressure",            0x71, 0x70 },
    { "Off",                 0x71, 0x78 }
};

static ScaleParams mod1_depth_params = {
    .parameter = 0x72,
    .offset = 64
};

static ComboBoxEntry mod2_srcs[] = {
    { "LFO 1",               0x73, 0x00 },
    { "LFO 2",               0x73, 0x08 },
    { "LFO 3",               0x73, 0x10 },
    { "Envelope 1",          0x73, 0x18 },
    { "Envelope 2",          0x73, 0x20 },
    { "Envelope 3",          0x73, 0x28 },
    { "Envelope 4",          0x73, 0x30 },
    { "Velocity",            0x73, 0x38 },
    { "Velocity X",          0x73, 0x40 },
    { "Keyboard",            0x73, 0x48 },
    { "Keyboard 2",          0x73, 0x50 },
    { "Modulation Wheel",    0x73, 0x58 },
    { "Foot Pedal",          0x73, 0x60 },
    { "External Controller", 0x73, 0x68 },
    { "Pressure",            0x73, 0x70 },
    { "Off",                 0x73, 0x78 }
};

static ScaleParams mod2_depth_params = {
    .parameter = 0x74,
    .offset = 64
};

FilterDialog *
new_filter_dialog(GtkWindow *parent)
{
    FilterDialog *widgets;
    GtkGrid *grid;
    GtkWidget *label, *button_box, *button;

    widgets = g_new(FilterDialog, 1);

    widgets->dialog = create_window(parent, "Filter", FALSE);

    grid = create_grid(GTK_CONTAINER(widgets->dialog));

    label = gtk_label_new("Frequency:");
    widgets->frequency = create_hscale(0, 127, 0x6e);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(widgets->frequency));

    label = gtk_label_new("Resonance:");
    widgets->resonance = create_hscale_with_params(0, 31, &resonance_params);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(widgets->resonance));

    label = gtk_label_new("Keyboard Tracking:");
    widgets->keyboard_tracking = create_hscale_with_params(0, 63, &keyboard_tracking_params);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(widgets->keyboard_tracking));

    label = gtk_label_new("Mod 1:");
    widgets->mod1_src = create_combo_box_with_entries(mod1_srcs, G_N_ELEMENTS(mod1_srcs));
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(widgets->mod1_src));

    label = gtk_label_new("Mod 1 Depth:");
    widgets->mod1_depth = create_hscale_with_params(-63, 63, &mod1_depth_params);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(widgets->mod1_depth));

    label = gtk_label_new("Mod 2:");
    widgets->mod2_src = create_combo_box_with_entries(mod2_srcs, G_N_ELEMENTS(mod2_srcs));
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(widgets->mod2_src));

    label = gtk_label_new("Mod 2 Depth:");
    widgets->mod2_depth = create_hscale_with_params(-63, 63, &mod2_depth_params);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(widgets->mod2_depth));

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 6);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_grid_attach(grid, button_box, 0, 7, 2, 1);

    button = gtk_button_new_with_label("Close");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(close_window_callback), widgets->dialog);
    gtk_container_add(GTK_CONTAINER(button_box), button);

    return widgets;
}

void
show_filter_dialog(FilterDialog *widgets)
{
    show_window(widgets->dialog);
}

void
set_filter_parameters(FilterDialog *widgets, Patch *patch)
{
    printf("Setting filter parameters: frequency %d, resonance %d, keyboard tracking %d, mod1 src %d, mod1 depth %d, mod2 src %d, mod2 depth %d\n",
        patch->parameters[PARAMETER_FILTER_FREQUENCY],
        patch->parameters[PARAMETER_FILTER_RESONANCE],
        patch->parameters[PARAMETER_FILTER_KEYBOARD_TRACKING],
        patch->parameters[PARAMETER_FILTER_MOD1_SRC],
        patch->parameters[PARAMETER_FILTER_MOD1_DEPTH],
        patch->parameters[PARAMETER_FILTER_MOD2_SRC],
        patch->parameters[PARAMETER_FILTER_MOD2_DEPTH]);

    gtk_range_set_value(GTK_RANGE(widgets->frequency), patch->parameters[PARAMETER_FILTER_FREQUENCY]);
    gtk_range_set_value(GTK_RANGE(widgets->resonance), patch->parameters[PARAMETER_FILTER_RESONANCE]);
    gtk_range_set_value(GTK_RANGE(widgets->keyboard_tracking), patch->parameters[PARAMETER_FILTER_KEYBOARD_TRACKING]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->mod1_src), patch->parameters[PARAMETER_FILTER_MOD1_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->mod1_depth), patch->parameters[PARAMETER_FILTER_MOD1_DEPTH]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->mod2_src), patch->parameters[PARAMETER_FILTER_MOD2_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->mod2_depth), patch->parameters[PARAMETER_FILTER_MOD2_DEPTH]);
}

void
clear_filter_parameters(FilterDialog *widgets)
{
    gtk_range_set_value(GTK_RANGE(widgets->frequency), 127);
    gtk_range_set_value(GTK_RANGE(widgets->resonance), 0);
    gtk_range_set_value(GTK_RANGE(widgets->keyboard_tracking), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->mod1_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->mod1_depth), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->mod2_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->mod2_depth), 0);
}
