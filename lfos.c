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
#include "lfos.h"

static ScaleParams lfo1_frequency_params = {
    .parameter = PARAMETER_LFO1_FREQUENCY,
    .multiplier = 2
};

static ComboBoxEntry lfo1_waves[] = {
    { "Triangle", PARAMETER_LFO1_WAVE, 0x00 },
    { "Sawtooth", PARAMETER_LFO1_WAVE, 0x20 },
    { "Square",   PARAMETER_LFO1_WAVE, 0x40 },
    { "Noise",    PARAMETER_LFO1_WAVE, 0x60 }
};

static ScaleParams lfo1_initial_level_params = {
    .parameter = PARAMETER_LFO1_INITIAL_LEVEL,
    .multiplier = 2
};

static ScaleParams lfo1_delay_params = {
    .parameter = PARAMETER_LFO1_DELAY,
    .multiplier = 2
};

static ScaleParams lfo1_final_level_params = {
    .parameter = PARAMETER_LFO1_FINAL_LEVEL,
    .multiplier = 2
};

static ComboBoxEntry lfo1_mod_srcs[] = {
    { "LFO 1",               PARAMETER_LFO1_MOD_SRC, 0x00 },
    { "LFO 2",               PARAMETER_LFO1_MOD_SRC, 0x08 },
    { "LFO 3",               PARAMETER_LFO1_MOD_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_LFO1_MOD_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_LFO1_MOD_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_LFO1_MOD_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_LFO1_MOD_SRC, 0x30 },
    { "Velocity",            PARAMETER_LFO1_MOD_SRC, 0x38 },
    { "Velocity X",          PARAMETER_LFO1_MOD_SRC, 0x40 },
    { "Keyboard",            PARAMETER_LFO1_MOD_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_LFO1_MOD_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_LFO1_MOD_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_LFO1_MOD_SRC, 0x60 },
    { "External Controller", PARAMETER_LFO1_MOD_SRC, 0x68 },
    { "Pressure",            PARAMETER_LFO1_MOD_SRC, 0x70 },
    { "Off",                 PARAMETER_LFO1_MOD_SRC, 0x78 }
};

static ScaleParams lfo2_frequency_params = {
    .parameter = PARAMETER_LFO2_FREQUENCY,
    .multiplier = 2
};

static ComboBoxEntry lfo2_waves[] = {
    { "Triangle", PARAMETER_LFO2_WAVE, 0x00 },
    { "Sawtooth", PARAMETER_LFO2_WAVE, 0x20 },
    { "Square",   PARAMETER_LFO2_WAVE, 0x40 },
    { "Noise",    PARAMETER_LFO2_WAVE, 0x60 }
};

static ScaleParams lfo2_initial_level_params = {
    .parameter = PARAMETER_LFO2_INITIAL_LEVEL,
    .multiplier = 2
};

static ScaleParams lfo2_delay_params = {
    .parameter = PARAMETER_LFO2_DELAY,
    .multiplier = 2
};

static ScaleParams lfo2_final_level_params = {
    .parameter = PARAMETER_LFO2_FINAL_LEVEL,
    .multiplier = 2
};

static ComboBoxEntry lfo2_mod_srcs[] = {
    { "LFO 1",               PARAMETER_LFO2_MOD_SRC, 0x00 },
    { "LFO 2",               PARAMETER_LFO2_MOD_SRC, 0x08 },
    { "LFO 3",               PARAMETER_LFO2_MOD_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_LFO2_MOD_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_LFO2_MOD_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_LFO2_MOD_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_LFO2_MOD_SRC, 0x30 },
    { "Velocity",            PARAMETER_LFO2_MOD_SRC, 0x38 },
    { "Velocity X",          PARAMETER_LFO2_MOD_SRC, 0x40 },
    { "Keyboard",            PARAMETER_LFO2_MOD_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_LFO2_MOD_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_LFO2_MOD_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_LFO2_MOD_SRC, 0x60 },
    { "External Controller", PARAMETER_LFO2_MOD_SRC, 0x68 },
    { "Pressure",            PARAMETER_LFO2_MOD_SRC, 0x70 },
    { "Off",                 PARAMETER_LFO2_MOD_SRC, 0x78 }
};

static ScaleParams lfo3_frequency_params = {
    .parameter = PARAMETER_LFO3_FREQUENCY,
    .multiplier = 2
};

static ComboBoxEntry lfo3_waves[] = {
    { "Triangle", PARAMETER_LFO3_WAVE, 0x00 },
    { "Sawtooth", PARAMETER_LFO3_WAVE, 0x20 },
    { "Square",   PARAMETER_LFO3_WAVE, 0x40 },
    { "Noise",    PARAMETER_LFO3_WAVE, 0x60 }
};

static ScaleParams lfo3_initial_level_params = {
    .parameter = PARAMETER_LFO3_INITIAL_LEVEL,
    .multiplier = 2
};

static ScaleParams lfo3_delay_params = {
    .parameter = PARAMETER_LFO3_DELAY,
    .multiplier = 2
};

static ScaleParams lfo3_final_level_params = {
    .parameter = PARAMETER_LFO3_FINAL_LEVEL,
    .multiplier = 2
};

static ComboBoxEntry lfo3_mod_srcs[] = {
    { "LFO 1",               PARAMETER_LFO3_MOD_SRC, 0x00 },
    { "LFO 2",               PARAMETER_LFO3_MOD_SRC, 0x08 },
    { "LFO 3",               PARAMETER_LFO3_MOD_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_LFO3_MOD_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_LFO3_MOD_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_LFO3_MOD_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_LFO3_MOD_SRC, 0x30 },
    { "Velocity",            PARAMETER_LFO3_MOD_SRC, 0x38 },
    { "Velocity X",          PARAMETER_LFO3_MOD_SRC, 0x40 },
    { "Keyboard",            PARAMETER_LFO3_MOD_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_LFO3_MOD_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_LFO3_MOD_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_LFO3_MOD_SRC, 0x60 },
    { "External Controller", PARAMETER_LFO3_MOD_SRC, 0x68 },
    { "Pressure",            PARAMETER_LFO3_MOD_SRC, 0x70 },
    { "Off",                 PARAMETER_LFO3_MOD_SRC, 0x78 }
};

static GtkWidget *create_lfo1(Lfo *);
static GtkWidget *create_lfo2(Lfo *);
static GtkWidget *create_lfo3(Lfo *);

LfosDialog *
new_lfos_dialog(GtkWindow *parent)
{
    LfosDialog *widgets;
    GtkGrid *grid;
    GtkWidget *button_box, *button;

    widgets = g_new(LfosDialog, 1);

    widgets->dialog = create_window(parent, "LFOs", FALSE);

    grid = create_grid(GTK_CONTAINER(widgets->dialog));

    gtk_grid_attach(grid, create_lfo1(&widgets->lfo1), 0, 0, 1, 1);
    gtk_grid_attach(grid, create_lfo2(&widgets->lfo2), 1, 0, 1, 1);
    gtk_grid_attach(grid, create_lfo3(&widgets->lfo3), 2, 0, 1, 1);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 6);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_grid_attach(grid, button_box, 0, 1, 3, 1);

    button = gtk_button_new_with_label("Close");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(close_window_callback), widgets->dialog);
    gtk_container_add(GTK_CONTAINER(button_box), button);

    return widgets;
}

void
show_lfos_dialog(LfosDialog *widgets)
{
    show_window(widgets->dialog);
}

void
set_lfos_parameters(LfosDialog *widgets, Patch *patch)
{
    printf("Setting LFO1 parameters: frequency %d, reset %d, human %d, wave %d, initial level %d, delay %d, final level %d, mod src %d\n",
        patch->parameters[PARAMETER_LFO1_FREQUENCY],
        patch->parameters[PARAMETER_LFO1_RESET],
        patch->parameters[PARAMETER_LFO1_HUMAN],
        patch->parameters[PARAMETER_LFO1_WAVE],
        patch->parameters[PARAMETER_LFO1_INITIAL_LEVEL],
        patch->parameters[PARAMETER_LFO1_DELAY],
        patch->parameters[PARAMETER_LFO1_FINAL_LEVEL],
        patch->parameters[PARAMETER_LFO1_MOD_SRC]);

    gtk_range_set_value(GTK_RANGE(widgets->lfo1.frequency), patch->parameters[PARAMETER_LFO1_FREQUENCY]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo1.reset), patch->parameters[PARAMETER_LFO1_RESET] ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo1.human), patch->parameters[PARAMETER_LFO1_HUMAN] ? TRUE : FALSE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo1.wave), patch->parameters[PARAMETER_LFO1_WAVE]);
    gtk_range_set_value(GTK_RANGE(widgets->lfo1.initial_level), patch->parameters[PARAMETER_LFO1_INITIAL_LEVEL]);
    gtk_range_set_value(GTK_RANGE(widgets->lfo1.delay), patch->parameters[PARAMETER_LFO1_DELAY]);
    gtk_range_set_value(GTK_RANGE(widgets->lfo1.final_level), patch->parameters[PARAMETER_LFO1_FINAL_LEVEL]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo1.mod_src), patch->parameters[PARAMETER_LFO1_MOD_SRC]);

    printf("Setting LFO2 parameters: frequency %d, reset %d, human %d, wave %d, initial level %d, delay %d, final level %d, mod src %d\n",
        patch->parameters[PARAMETER_LFO2_FREQUENCY],
        patch->parameters[PARAMETER_LFO2_RESET],
        patch->parameters[PARAMETER_LFO2_HUMAN],
        patch->parameters[PARAMETER_LFO2_WAVE],
        patch->parameters[PARAMETER_LFO2_INITIAL_LEVEL],
        patch->parameters[PARAMETER_LFO2_DELAY],
        patch->parameters[PARAMETER_LFO2_FINAL_LEVEL],
        patch->parameters[PARAMETER_LFO2_MOD_SRC]);

    gtk_range_set_value(GTK_RANGE(widgets->lfo2.frequency), patch->parameters[PARAMETER_LFO2_FREQUENCY]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo2.reset), patch->parameters[PARAMETER_LFO2_RESET] ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo2.human), patch->parameters[PARAMETER_LFO2_HUMAN] ? TRUE : FALSE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo2.wave), patch->parameters[PARAMETER_LFO2_WAVE]);
    gtk_range_set_value(GTK_RANGE(widgets->lfo2.initial_level), patch->parameters[PARAMETER_LFO2_INITIAL_LEVEL]);
    gtk_range_set_value(GTK_RANGE(widgets->lfo2.delay), patch->parameters[PARAMETER_LFO2_DELAY]);
    gtk_range_set_value(GTK_RANGE(widgets->lfo2.final_level), patch->parameters[PARAMETER_LFO2_FINAL_LEVEL]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo2.mod_src), patch->parameters[PARAMETER_LFO2_MOD_SRC]);

    printf("Setting LFO3 parameters: frequency %d, reset %d, human %d, wave %d, initial level %d, delay %d, final level %d, mod src %d\n",
        patch->parameters[PARAMETER_LFO3_FREQUENCY],
        patch->parameters[PARAMETER_LFO3_RESET],
        patch->parameters[PARAMETER_LFO3_HUMAN],
        patch->parameters[PARAMETER_LFO3_WAVE],
        patch->parameters[PARAMETER_LFO3_INITIAL_LEVEL],
        patch->parameters[PARAMETER_LFO3_DELAY],
        patch->parameters[PARAMETER_LFO3_FINAL_LEVEL],
        patch->parameters[PARAMETER_LFO3_MOD_SRC]);

    gtk_range_set_value(GTK_RANGE(widgets->lfo3.frequency), patch->parameters[PARAMETER_LFO3_FREQUENCY]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo3.reset), patch->parameters[PARAMETER_LFO3_RESET] ? TRUE : FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo3.human), patch->parameters[PARAMETER_LFO3_HUMAN] ? TRUE : FALSE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo3.wave), patch->parameters[PARAMETER_LFO3_WAVE]);
    gtk_range_set_value(GTK_RANGE(widgets->lfo3.initial_level), patch->parameters[PARAMETER_LFO3_INITIAL_LEVEL]);
    gtk_range_set_value(GTK_RANGE(widgets->lfo3.delay), patch->parameters[PARAMETER_LFO3_DELAY]);
    gtk_range_set_value(GTK_RANGE(widgets->lfo3.final_level), patch->parameters[PARAMETER_LFO3_FINAL_LEVEL]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo3.mod_src), patch->parameters[PARAMETER_LFO3_MOD_SRC]);
}

void
clear_lfos_parameters(LfosDialog *widgets)
{
    gtk_range_set_value(GTK_RANGE(widgets->lfo1.frequency), 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo1.reset), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo1.human), FALSE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo1.wave), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lfo1.initial_level), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lfo1.delay), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lfo1.final_level), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo1.mod_src), 0);

    gtk_range_set_value(GTK_RANGE(widgets->lfo2.frequency), 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo2.reset), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo2.human), FALSE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo2.wave), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lfo2.initial_level), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lfo2.delay), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lfo2.final_level), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo2.mod_src), 0);

    gtk_range_set_value(GTK_RANGE(widgets->lfo3.frequency), 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo3.reset), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lfo3.human), FALSE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo3.wave), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lfo3.initial_level), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lfo3.delay), 0);
    gtk_range_set_value(GTK_RANGE(widgets->lfo3.final_level), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->lfo3.mod_src), 0);
}

static GtkWidget *
create_lfo1(Lfo *lfo)
{
    GtkWidget *frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("LFO 1");

    grid = create_grid(GTK_CONTAINER(frame));

    label = gtk_label_new("Frequency:");
    lfo->frequency = create_hscale_with_params(0, 63, &lfo1_frequency_params);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(lfo->frequency));

    label = gtk_label_new("Reset:");
    lfo->reset = create_check_button(PARAMETER_LFO1_RESET);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(lfo->reset));

    label = gtk_label_new("Human:");
    lfo->human = create_check_button(PARAMETER_LFO1_HUMAN);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(lfo->human));

    label = gtk_label_new("Wave:");
    lfo->wave = create_combo_box_with_entries(lfo1_waves, G_N_ELEMENTS(lfo1_waves));
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(lfo->wave));

    label = gtk_label_new("Initial Level:");
    lfo->initial_level = create_hscale_with_params(0, 63, &lfo1_initial_level_params);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(lfo->initial_level));

    label = gtk_label_new("Delay:");
    lfo->delay = create_hscale_with_params(0, 63, &lfo1_delay_params);
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(lfo->delay));

    label = gtk_label_new("Final Level:");
    lfo->final_level = create_hscale_with_params(0, 63, &lfo1_final_level_params);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(lfo->final_level));

    label = gtk_label_new("Mod:");
    lfo->mod_src = create_combo_box_with_entries(lfo1_mod_srcs, G_N_ELEMENTS(lfo1_mod_srcs));
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(lfo->mod_src));

    return frame;
}

static GtkWidget *
create_lfo2(Lfo *lfo)
{
    GtkWidget *frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("LFO 2");

    grid = create_grid(GTK_CONTAINER(frame));

    label = gtk_label_new("Frequency:");
    lfo->frequency = create_hscale_with_params(0, 63, &lfo2_frequency_params);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(lfo->frequency));

    label = gtk_label_new("Reset:");
    lfo->reset = create_check_button(PARAMETER_LFO2_RESET);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(lfo->reset));

    label = gtk_label_new("Human:");
    lfo->human = create_check_button(PARAMETER_LFO2_HUMAN);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(lfo->human));

    label = gtk_label_new("Wave:");
    lfo->wave = create_combo_box_with_entries(lfo2_waves, G_N_ELEMENTS(lfo2_waves));
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(lfo->wave));

    label = gtk_label_new("Initial Level:");
    lfo->initial_level = create_hscale_with_params(0, 63, &lfo2_initial_level_params);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(lfo->initial_level));

    label = gtk_label_new("Delay:");
    lfo->delay = create_hscale_with_params(0, 63, &lfo2_delay_params);
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(lfo->delay));

    label = gtk_label_new("Final Level:");
    lfo->final_level = create_hscale_with_params(0, 63, &lfo2_final_level_params);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(lfo->final_level));

    label = gtk_label_new("Mod:");
    lfo->mod_src = create_combo_box_with_entries(lfo2_mod_srcs, G_N_ELEMENTS(lfo2_mod_srcs));
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(lfo->mod_src));

    return frame;
}

static GtkWidget *
create_lfo3(Lfo *lfo)
{
    GtkWidget *frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("LFO 3");

    grid = create_grid(GTK_CONTAINER(frame));

    label = gtk_label_new("Frequency:");
    lfo->frequency = create_hscale_with_params(0, 63, &lfo3_frequency_params);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(lfo->frequency));

    label = gtk_label_new("Reset:");
    lfo->reset = create_check_button(PARAMETER_LFO3_RESET);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(lfo->reset));

    label = gtk_label_new("Human:");
    lfo->human = create_check_button(PARAMETER_LFO3_HUMAN);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(lfo->human));

    label = gtk_label_new("Wave:");
    lfo->wave = create_combo_box_with_entries(lfo3_waves, G_N_ELEMENTS(lfo3_waves));
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(lfo->wave));

    label = gtk_label_new("Initial Level:");
    lfo->initial_level = create_hscale_with_params(0, 63, &lfo3_initial_level_params);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(lfo->initial_level));

    label = gtk_label_new("Delay:");
    lfo->delay = create_hscale_with_params(0, 63, &lfo3_delay_params);
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(lfo->delay));

    label = gtk_label_new("Final Level:");
    lfo->final_level = create_hscale_with_params(0, 63, &lfo3_final_level_params);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(lfo->final_level));

    label = gtk_label_new("Mod:");
    lfo->mod_src = create_combo_box_with_entries(lfo3_mod_srcs, G_N_ELEMENTS(lfo3_mod_srcs));
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(lfo->mod_src));

    return frame;
}
