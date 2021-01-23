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
#include "modes.h"

static ScaleParams glide_params = {
    .parameter = PARAMETER_GLIDE,
    .multiplier = 2
};

ModesDialog *
new_modes_dialog(GtkWindow *parent)
{
    ModesDialog *widgets;
    GtkGrid *grid;
    GtkWidget *label, *button_box, *button;

    widgets = g_new(ModesDialog, 1);

    widgets->dialog = create_window(parent, "Modes", FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(widgets->dialog), 300, -1);

    grid = create_grid(GTK_CONTAINER(widgets->dialog));

    label = gtk_label_new("Amplitude Modulation:");
    widgets->amplitude_modulation = create_check_button(PARAMETER_AMPLITUDE_MODULATION);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(widgets->amplitude_modulation));

    label = gtk_label_new("Glide:");
    widgets->glide = create_hscale_with_params(0, 63, &glide_params);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(widgets->glide));

    label = gtk_label_new("Mono:");
    widgets->mono = create_check_button(PARAMETER_MONO);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(widgets->mono));

    label = gtk_label_new("Sync:");
    widgets->sync = create_check_button(PARAMETER_SYNC);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(widgets->sync));

    label = gtk_label_new("Voice Restart:");
    widgets->voice_restart = create_check_button(PARAMETER_VOICE_RESTART);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(widgets->voice_restart));

    label = gtk_label_new("Envelope Restart:");
    widgets->envelope_restart = create_check_button(PARAMETER_ENVELOPE_RESTART);
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(widgets->envelope_restart));

    label = gtk_label_new("Oscillator Restart:");
    widgets->oscillator_restart = create_check_button(PARAMETER_OSCILLATOR_RESTART);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(widgets->oscillator_restart));

    label = gtk_label_new("Envelope Full Cycle:");
    widgets->envelope_full_cycle = create_check_button(PARAMETER_ENVELOPE_FULL_CYCLE);
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(widgets->envelope_full_cycle));

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 6);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_grid_attach(grid, button_box, 0, 8, 2, 1);

    button = gtk_button_new_with_label("Close");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(close_window_callback), widgets->dialog);
    gtk_container_add(GTK_CONTAINER(button_box), button);

    return widgets;
}

void
show_modes_dialog(ModesDialog *widgets)
{
    show_window(widgets->dialog);
}

void
set_modes_parameters(ModesDialog *widgets, Patch *patch)
{
    printf("Setting modes parameters: amp mod %d, glide %d, mono %d, sync %d, voice restart %d, env restart %d, osc restart %d, env full cycle %d\n",
        patch->parameters[PARAMETER_AMPLITUDE_MODULATION],
        patch->parameters[PARAMETER_GLIDE],
        patch->parameters[PARAMETER_MONO],
        patch->parameters[PARAMETER_SYNC],
        patch->parameters[PARAMETER_VOICE_RESTART],
        patch->parameters[PARAMETER_ENVELOPE_RESTART],
        patch->parameters[PARAMETER_OSCILLATOR_RESTART],
        patch->parameters[PARAMETER_ENVELOPE_FULL_CYCLE]);

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->amplitude_modulation), patch->parameters[PARAMETER_AMPLITUDE_MODULATION] ? TRUE : FALSE);
    gtk_range_set_value(GTK_RANGE(widgets->glide), patch->parameters[PARAMETER_GLIDE]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->mono), patch->parameters[PARAMETER_MONO] ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->sync), patch->parameters[PARAMETER_SYNC] ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->voice_restart), patch->parameters[PARAMETER_VOICE_RESTART] ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->envelope_restart), patch->parameters[PARAMETER_ENVELOPE_RESTART] ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->oscillator_restart), patch->parameters[PARAMETER_OSCILLATOR_RESTART] ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->envelope_full_cycle), patch->parameters[PARAMETER_ENVELOPE_FULL_CYCLE] ? TRUE : FALSE);
}

void
clear_modes_parameters(ModesDialog *widgets)
{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->amplitude_modulation), FALSE);
    gtk_range_set_value(GTK_RANGE(widgets->glide), 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->mono), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->sync), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->voice_restart), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->envelope_restart), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->oscillator_restart), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->envelope_full_cycle), FALSE);
}
