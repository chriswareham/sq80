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
#include "amplifier.h"

static ScaleParams env4_depth_params = {
    .parameter = 0x6a,
    .multiplier = 2
};

static ScaleParams pan_params = {
    .parameter = 0x6b,
    .multiplier = 8
};

static ComboBoxEntry mod_srcs[] = {
    { "LFO 1",               0x6c, 0x00 },
    { "LFO 2",               0x6c, 0x08 },
    { "LFO 3",               0x6c, 0x10 },
    { "Envelope 1",          0x6c, 0x18 },
    { "Envelope 2",          0x6c, 0x20 },
    { "Envelope 3",          0x6c, 0x28 },
    { "Envelope 4",          0x6c, 0x30 },
    { "Velocity",            0x6c, 0x38 },
    { "Velocity X",          0x6c, 0x40 },
    { "Keyboard",            0x6c, 0x48 },
    { "Keyboard 2",          0x6c, 0x50 },
    { "Modulation Wheel",    0x6c, 0x58 },
    { "Foot Pedal",          0x6c, 0x60 },
    { "External Controller", 0x6c, 0x68 },
    { "Pressure",            0x6c, 0x70 },
    { "Off",                 0x6c, 0x78 }
};

static ScaleParams pan_mod_depth_params = {
    .parameter = 0x6d,
    .offset = 64
};

AmplifierDialog *
new_amplifier_dialog(GtkWindow *parent)
{
    AmplifierDialog *widgets;
    GtkGrid *grid;
    GtkWidget *label, *button_box, *button;

    widgets = g_new(AmplifierDialog, 1);

    widgets->dialog = create_window(parent, "Amplifier", FALSE);

    grid = create_grid(GTK_CONTAINER(widgets->dialog));

    label = gtk_label_new("Env 4 Depth:");
    widgets->env4_depth = create_hscale_with_params(0, 63, &env4_depth_params);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(widgets->env4_depth));

    label = gtk_label_new("Pan:");
    widgets->pan = create_hscale_with_params(0, 15, &pan_params);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(widgets->pan));

    label = gtk_label_new("Pan Mod:");
    widgets->mod_src = create_combo_box_with_entries(mod_srcs, G_N_ELEMENTS(mod_srcs));
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(widgets->mod_src));

    label = gtk_label_new("Pan Mod Depth:");
    widgets->mod_depth = create_hscale_with_params(-63, 63, &pan_mod_depth_params);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(widgets->mod_depth));

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 6);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_grid_attach(grid, button_box, 0, 4, 2, 1);

    button = gtk_button_new_with_label("Close");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(close_window_callback), widgets->dialog);
    gtk_container_add(GTK_CONTAINER(button_box), button);

    return widgets;
}

void
show_amplifier_dialog(AmplifierDialog *widgets)
{
    show_window(widgets->dialog);
}

void
set_amplifier_parameters(AmplifierDialog *widgets, Patch *patch)
{
    printf("Setting amplifier parameters: pan %d, env4 depth %d, mod src %d, mod depth %d\n",
        patch->parameters[PARAMETER_DCA4_PAN],
        patch->parameters[PARAMETER_DCA4_ENV4_DEPTH],
        patch->parameters[PARAMETER_DCA4_MOD_SRC],
        patch->parameters[PARAMETER_DCA4_MOD_DEPTH]);

    gtk_range_set_value(GTK_RANGE(widgets->pan), patch->parameters[PARAMETER_DCA4_PAN]);
    gtk_range_set_value(GTK_RANGE(widgets->env4_depth), patch->parameters[PARAMETER_DCA4_ENV4_DEPTH]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->mod_src), patch->parameters[PARAMETER_DCA4_MOD_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->mod_depth), patch->parameters[PARAMETER_DCA4_MOD_DEPTH]);
}

void
clear_amplifier_parameters(AmplifierDialog *widgets)
{
    gtk_range_set_value(GTK_RANGE(widgets->pan), 8);
    gtk_range_set_value(GTK_RANGE(widgets->env4_depth), 63);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->mod_src), 15);
    gtk_range_set_value(GTK_RANGE(widgets->mod_depth), 0);
}
