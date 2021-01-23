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
#include "oscillators.h"

static gchar *waves[] = {
    /* Following waves are common to the ESQ-1 and SQ-80 */
    "Sawtooth",
    "Bell",
    "Sine",
    "Square",
    "Pulse",
    "Noise 1",
    "Noise 2",
    "Noise 3",
    "Bass",
    "Piano",
    "Electric Piano",
    "Voice 1",
    "Voice 2",
    "Kick",
    "Reed",
    "Organ",
    "Synth 1",
    "Synth 2",
    "Synth 3",
    "Formant 1",
    "Formant 2",
    "Formant 3",
    "Formant 4",
    "Formant 5",
    "Pulse 2",
    "Square 2",
    "Four Octaves",
    "Prime",
    "Bass 2",
    "Electric Piano 2",
    "Octave",
    "Octave And Fifth",
    /* Following waves are specific to the SQ-80 */
    "Sawtooth 2",
    "Triangle",
    "Reed 2",
    "Reed 3",
    "Grit 1",
    "Grit 2",
    "Grit 3",
    "Glint 1",
    "Glint 2",
    "Glint 3",
    "Clav",
    "Brass",
    "String",
    "Digit 1",
    "Digit 2",
    "Bell 2",
    "Alien",
    "Breath",
    "Voice 3",
    "Steam",
    "Metal",
    "Chime",
    "Bowing",
    "Pick 1",
    "Pick 2",
    "Mallet",
    "Slap",
    "Plink",
    "Pluck",
    "Plunk",
    "Click",
    "Chiff",
    "Thump",
    "Log Drum",
    "Kick 2",
    "Snare",
    "Tom Tom",
    "Hi Hat",
    "Drums 1",
    "Drums 2",
    "Drums 3",
    "Drums 4",
    "Drums 5"
};

static ComboBoxEntry osc1_mod1_srcs[] = {
    { "LFO 1",               PARAMETER_OSC1_MOD1_SRC, 0x00 },
    { "LFO 2",               PARAMETER_OSC1_MOD1_SRC, 0x08 },
    { "LFO 3",               PARAMETER_OSC1_MOD1_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_OSC1_MOD1_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_OSC1_MOD1_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_OSC1_MOD1_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_OSC1_MOD1_SRC, 0x30 },
    { "Velocity",            PARAMETER_OSC1_MOD1_SRC, 0x38 },
    { "Velocity X",          PARAMETER_OSC1_MOD1_SRC, 0x40 },
    { "Keyboard",            PARAMETER_OSC1_MOD1_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_OSC1_MOD1_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_OSC1_MOD1_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_OSC1_MOD1_SRC, 0x60 },
    { "External Controller", PARAMETER_OSC1_MOD1_SRC, 0x68 },
    { "Pressure",            PARAMETER_OSC1_MOD1_SRC, 0x70 },
    { "Off",                 PARAMETER_OSC1_MOD1_SRC, 0x78 }
};

static ScaleParams osc1_mod1_depth_params = {
    .parameter = PARAMETER_OSC1_MOD1_DEPTH,
    .offset = 64
};

static ComboBoxEntry osc1_mod2_srcs[] = {
    { "LFO 1",               PARAMETER_OSC1_MOD2_SRC, 0x00 },
    { "LFO 2",               PARAMETER_OSC1_MOD2_SRC, 0x08 },
    { "LFO 3",               PARAMETER_OSC1_MOD2_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_OSC1_MOD2_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_OSC1_MOD2_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_OSC1_MOD2_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_OSC1_MOD2_SRC, 0x30 },
    { "Velocity",            PARAMETER_OSC1_MOD2_SRC, 0x38 },
    { "Velocity X",          PARAMETER_OSC1_MOD2_SRC, 0x40 },
    { "Keyboard",            PARAMETER_OSC1_MOD2_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_OSC1_MOD2_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_OSC1_MOD2_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_OSC1_MOD2_SRC, 0x60 },
    { "External Controller", PARAMETER_OSC1_MOD2_SRC, 0x68 },
    { "Pressure",            PARAMETER_OSC1_MOD2_SRC, 0x70 },
    { "Off",                 PARAMETER_OSC1_MOD2_SRC, 0x78 }
};

static ScaleParams osc1_mod2_depth_params = {
    .parameter = PARAMETER_OSC1_MOD2_DEPTH,
    .offset = 64
};

static ComboBoxEntry osc2_mod1_srcs[] = {
    { "LFO 1",               PARAMETER_OSC2_MOD1_SRC, 0x00 },
    { "LFO 2",               PARAMETER_OSC2_MOD1_SRC, 0x08 },
    { "LFO 3",               PARAMETER_OSC2_MOD1_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_OSC2_MOD1_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_OSC2_MOD1_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_OSC2_MOD1_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_OSC2_MOD1_SRC, 0x30 },
    { "Velocity",            PARAMETER_OSC2_MOD1_SRC, 0x38 },
    { "Velocity X",          PARAMETER_OSC2_MOD1_SRC, 0x40 },
    { "Keyboard",            PARAMETER_OSC2_MOD1_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_OSC2_MOD1_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_OSC2_MOD1_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_OSC2_MOD1_SRC, 0x60 },
    { "External Controller", PARAMETER_OSC2_MOD1_SRC, 0x68 },
    { "Pressure",            PARAMETER_OSC2_MOD1_SRC, 0x70 },
    { "Off",                 PARAMETER_OSC2_MOD1_SRC, 0x78 }
};

static ScaleParams osc2_mod1_depth_params = {
    .parameter = PARAMETER_OSC2_MOD1_DEPTH,
    .offset = 64
};

static ComboBoxEntry osc2_mod2_srcs[] = {
    { "LFO 1",               PARAMETER_OSC2_MOD2_SRC, 0x00 },
    { "LFO 2",               PARAMETER_OSC2_MOD2_SRC, 0x08 },
    { "LFO 3",               PARAMETER_OSC2_MOD2_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_OSC2_MOD2_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_OSC2_MOD2_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_OSC2_MOD2_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_OSC2_MOD2_SRC, 0x30 },
    { "Velocity",            PARAMETER_OSC2_MOD2_SRC, 0x38 },
    { "Velocity X",          PARAMETER_OSC2_MOD2_SRC, 0x40 },
    { "Keyboard",            PARAMETER_OSC2_MOD2_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_OSC2_MOD2_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_OSC2_MOD2_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_OSC2_MOD2_SRC, 0x60 },
    { "External Controller", PARAMETER_OSC2_MOD2_SRC, 0x68 },
    { "Pressure",            PARAMETER_OSC2_MOD2_SRC, 0x70 },
    { "Off",                 PARAMETER_OSC2_MOD2_SRC, 0x78 }
};

static ScaleParams osc2_mod2_depth_params = {
    .parameter = PARAMETER_OSC2_MOD2_DEPTH,
    .offset = 64
};

static ComboBoxEntry osc3_mod1_srcs[] = {
    { "LFO 1",               PARAMETER_OSC3_MOD1_SRC, 0x00 },
    { "LFO 2",               PARAMETER_OSC3_MOD1_SRC, 0x08 },
    { "LFO 3",               PARAMETER_OSC3_MOD1_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_OSC3_MOD1_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_OSC3_MOD1_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_OSC3_MOD1_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_OSC3_MOD1_SRC, 0x30 },
    { "Velocity",            PARAMETER_OSC3_MOD1_SRC, 0x38 },
    { "Velocity X",          PARAMETER_OSC3_MOD1_SRC, 0x40 },
    { "Keyboard",            PARAMETER_OSC3_MOD1_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_OSC3_MOD1_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_OSC3_MOD1_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_OSC3_MOD1_SRC, 0x60 },
    { "External Controller", PARAMETER_OSC3_MOD1_SRC, 0x68 },
    { "Pressure",            PARAMETER_OSC3_MOD1_SRC, 0x70 },
    { "Off",                 PARAMETER_OSC3_MOD1_SRC, 0x78 }
};

static ScaleParams osc3_mod1_depth_params = {
    .parameter = PARAMETER_OSC3_MOD1_DEPTH,
    .offset = 64
};

static ComboBoxEntry osc3_mod2_srcs[] = {
    { "LFO 1",               PARAMETER_OSC3_MOD2_SRC, 0x00 },
    { "LFO 2",               PARAMETER_OSC3_MOD2_SRC, 0x08 },
    { "LFO 3",               PARAMETER_OSC3_MOD2_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_OSC3_MOD2_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_OSC3_MOD2_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_OSC3_MOD2_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_OSC3_MOD2_SRC, 0x30 },
    { "Velocity",            PARAMETER_OSC3_MOD2_SRC, 0x38 },
    { "Velocity X",          PARAMETER_OSC3_MOD2_SRC, 0x40 },
    { "Keyboard",            PARAMETER_OSC3_MOD2_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_OSC3_MOD2_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_OSC3_MOD2_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_OSC3_MOD2_SRC, 0x60 },
    { "External Controller", PARAMETER_OSC3_MOD2_SRC, 0x68 },
    { "Pressure",            PARAMETER_OSC3_MOD2_SRC, 0x70 },
    { "Off",                 PARAMETER_OSC3_MOD2_SRC, 0x78 }
};

static ScaleParams osc3_mod2_depth_params = {
    .parameter = PARAMETER_OSC3_MOD2_DEPTH,
    .offset = 64
};

static ComboBoxEntry dca1_mod1_srcs[] = {
    { "LFO 1",               PARAMETER_DCA1_MOD1_SRC, 0x00 },
    { "LFO 2",               PARAMETER_DCA1_MOD1_SRC, 0x08 },
    { "LFO 3",               PARAMETER_DCA1_MOD1_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_DCA1_MOD1_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_DCA1_MOD1_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_DCA1_MOD1_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_DCA1_MOD1_SRC, 0x30 },
    { "Velocity",            PARAMETER_DCA1_MOD1_SRC, 0x38 },
    { "Velocity X",          PARAMETER_DCA1_MOD1_SRC, 0x40 },
    { "Keyboard",            PARAMETER_DCA1_MOD1_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_DCA1_MOD1_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_DCA1_MOD1_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_DCA1_MOD1_SRC, 0x60 },
    { "External Controller", PARAMETER_DCA1_MOD1_SRC, 0x68 },
    { "Pressure",            PARAMETER_DCA1_MOD1_SRC, 0x70 },
    { "Off",                 PARAMETER_DCA1_MOD1_SRC, 0x78 }
};

static ScaleParams dca1_mod1_depth_params = {
    .parameter = PARAMETER_DCA1_MOD1_DEPTH,
    .offset = 64
};

static ComboBoxEntry dca1_mod2_srcs[] = {
    { "LFO 1",               PARAMETER_DCA1_MOD2_SRC, 0x00 },
    { "LFO 2",               PARAMETER_DCA1_MOD2_SRC, 0x08 },
    { "LFO 3",               PARAMETER_DCA1_MOD2_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_DCA1_MOD2_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_DCA1_MOD2_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_DCA1_MOD2_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_DCA1_MOD2_SRC, 0x30 },
    { "Velocity",            PARAMETER_DCA1_MOD2_SRC, 0x38 },
    { "Velocity X",          PARAMETER_DCA1_MOD2_SRC, 0x40 },
    { "Keyboard",            PARAMETER_DCA1_MOD2_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_DCA1_MOD2_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_DCA1_MOD2_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_DCA1_MOD2_SRC, 0x60 },
    { "External Controller", PARAMETER_DCA1_MOD2_SRC, 0x68 },
    { "Pressure",            PARAMETER_DCA1_MOD2_SRC, 0x70 },
    { "Off",                 PARAMETER_DCA1_MOD2_SRC, 0x78 }
};

static ScaleParams dca1_mod2_depth_params = {
    .parameter = PARAMETER_DCA2_MOD1_DEPTH,
    .offset = 64
};

static ComboBoxEntry dca2_mod1_srcs[] = {
    { "LFO 1",               PARAMETER_DCA2_MOD1_SRC, 0x00 },
    { "LFO 2",               PARAMETER_DCA2_MOD1_SRC, 0x08 },
    { "LFO 3",               PARAMETER_DCA2_MOD1_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_DCA2_MOD1_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_DCA2_MOD1_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_DCA2_MOD1_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_DCA2_MOD1_SRC, 0x30 },
    { "Velocity",            PARAMETER_DCA2_MOD1_SRC, 0x38 },
    { "Velocity X",          PARAMETER_DCA2_MOD1_SRC, 0x40 },
    { "Keyboard",            PARAMETER_DCA2_MOD1_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_DCA2_MOD1_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_DCA2_MOD1_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_DCA2_MOD1_SRC, 0x60 },
    { "External Controller", PARAMETER_DCA2_MOD1_SRC, 0x68 },
    { "Pressure",            PARAMETER_DCA2_MOD1_SRC, 0x70 },
    { "Off",                 PARAMETER_DCA2_MOD1_SRC, 0x78 }
};

static ScaleParams dca2_mod1_depth_params = {
    .parameter = PARAMETER_DCA2_MOD1_DEPTH,
    .offset = 64
};

static ComboBoxEntry dca2_mod2_srcs[] = {
    { "LFO 1",               PARAMETER_DCA2_MOD2_SRC, 0x00 },
    { "LFO 2",               PARAMETER_DCA2_MOD2_SRC, 0x08 },
    { "LFO 3",               PARAMETER_DCA2_MOD2_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_DCA2_MOD2_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_DCA2_MOD2_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_DCA2_MOD2_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_DCA2_MOD2_SRC, 0x30 },
    { "Velocity",            PARAMETER_DCA2_MOD2_SRC, 0x38 },
    { "Velocity X",          PARAMETER_DCA2_MOD2_SRC, 0x40 },
    { "Keyboard",            PARAMETER_DCA2_MOD2_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_DCA2_MOD2_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_DCA2_MOD2_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_DCA2_MOD2_SRC, 0x60 },
    { "External Controller", PARAMETER_DCA2_MOD2_SRC, 0x68 },
    { "Pressure",            PARAMETER_DCA2_MOD2_SRC, 0x70 },
    { "Off",                 PARAMETER_DCA2_MOD2_SRC, 0x78 }
};

static ScaleParams dca2_mod2_depth_params = {
    .parameter = PARAMETER_DCA2_MOD2_DEPTH,
    .offset = 64
};

static ComboBoxEntry dca3_mod1_srcs[] = {
    { "LFO 1",               PARAMETER_DCA3_MOD1_SRC, 0x00 },
    { "LFO 2",               PARAMETER_DCA3_MOD1_SRC, 0x08 },
    { "LFO 3",               PARAMETER_DCA3_MOD1_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_DCA3_MOD1_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_DCA3_MOD1_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_DCA3_MOD1_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_DCA3_MOD1_SRC, 0x30 },
    { "Velocity",            PARAMETER_DCA3_MOD1_SRC, 0x38 },
    { "Velocity X",          PARAMETER_DCA3_MOD1_SRC, 0x40 },
    { "Keyboard",            PARAMETER_DCA3_MOD1_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_DCA3_MOD1_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_DCA3_MOD1_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_DCA3_MOD1_SRC, 0x60 },
    { "External Controller", PARAMETER_DCA3_MOD1_SRC, 0x68 },
    { "Pressure",            PARAMETER_DCA3_MOD1_SRC, 0x70 },
    { "Off",                 PARAMETER_DCA3_MOD1_SRC, 0x78 }
};

static ScaleParams dca3_mod1_depth_params = {
    .parameter = PARAMETER_DCA3_MOD1_DEPTH,
    .offset = 64
};

static ComboBoxEntry dca3_mod2_srcs[] = {
    { "LFO 1",               PARAMETER_DCA3_MOD2_SRC, 0x00 },
    { "LFO 2",               PARAMETER_DCA3_MOD2_SRC, 0x08 },
    { "LFO 3",               PARAMETER_DCA3_MOD2_SRC, 0x10 },
    { "Envelope 1",          PARAMETER_DCA3_MOD2_SRC, 0x18 },
    { "Envelope 2",          PARAMETER_DCA3_MOD2_SRC, 0x20 },
    { "Envelope 3",          PARAMETER_DCA3_MOD2_SRC, 0x28 },
    { "Envelope 4",          PARAMETER_DCA3_MOD2_SRC, 0x30 },
    { "Velocity",            PARAMETER_DCA3_MOD2_SRC, 0x38 },
    { "Velocity X",          PARAMETER_DCA3_MOD2_SRC, 0x40 },
    { "Keyboard",            PARAMETER_DCA3_MOD2_SRC, 0x48 },
    { "Keyboard 2",          PARAMETER_DCA3_MOD2_SRC, 0x50 },
    { "Modulation Wheel",    PARAMETER_DCA3_MOD2_SRC, 0x58 },
    { "Foot Pedal",          PARAMETER_DCA3_MOD2_SRC, 0x60 },
    { "External Controller", PARAMETER_DCA3_MOD2_SRC, 0x68 },
    { "Pressure",            PARAMETER_DCA3_MOD2_SRC, 0x70 },
    { "Off",                 PARAMETER_DCA3_MOD2_SRC, 0x78 }
};

static ScaleParams dca3_mod2_depth_params = {
    .parameter = PARAMETER_DCA3_MOD2_DEPTH,
    .offset = 64
};

static GtkWidget *create_osc1(Oscillator *);
static GtkWidget *create_osc2(Oscillator *);
static GtkWidget *create_osc3(Oscillator *);
static GtkWidget *create_dca1(Oscillator *);
static GtkWidget *create_dca2(Oscillator *);
static GtkWidget *create_dca3(Oscillator *);

OscillatorsDialog *
new_oscillators_dialog(GtkWindow *parent)
{
    OscillatorsDialog *widgets;
    GtkGrid *grid;
    GtkWidget *button_box, *button;

    widgets = g_new(OscillatorsDialog, 1);

    widgets->dialog = create_window(parent, "Oscillators", FALSE);

    grid = create_grid(GTK_CONTAINER(widgets->dialog));

    gtk_grid_attach(grid, create_osc1(&widgets->osc1), 0, 0, 1, 1);
    gtk_grid_attach(grid, create_osc2(&widgets->osc2), 1, 0, 1, 1);
    gtk_grid_attach(grid, create_osc3(&widgets->osc3), 2, 0, 1, 1);

    gtk_grid_attach(grid, create_dca1(&widgets->osc1), 0, 1, 1, 1);
    gtk_grid_attach(grid, create_dca2(&widgets->osc2), 1, 1, 1, 1);
    gtk_grid_attach(grid, create_dca3(&widgets->osc3), 2, 1, 1, 1);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 6);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_grid_attach(grid, button_box, 0, 2, 3, 1);

    button = gtk_button_new_with_label("Close");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(close_window_callback), widgets->dialog);
    gtk_container_add(GTK_CONTAINER(button_box), button);

    return widgets;
}

void
show_oscillators_dialog(OscillatorsDialog *widgets)
{
    show_window(widgets->dialog);
}

void
set_oscillators_parameters(OscillatorsDialog *widgets, Patch *patch)
{
    printf("Setting OSC1 parameters: octave %d, semitone %d, fine %d, wave %d, mod src1 %d, mod1 depth %d, mod2 src %d, mod2 depth %d\n",
        patch->parameters[PARAMETER_OSC1_OCTAVE],
        patch->parameters[PARAMETER_OSC1_SEMITONE],
        patch->parameters[PARAMETER_OSC1_FINE],
        patch->parameters[PARAMETER_OSC1_WAVE],
        patch->parameters[PARAMETER_OSC1_MOD1_SRC],
        patch->parameters[PARAMETER_OSC1_MOD1_DEPTH],
        patch->parameters[PARAMETER_OSC1_MOD2_SRC],
        patch->parameters[PARAMETER_OSC1_MOD2_DEPTH]);

    gtk_range_set_value(GTK_RANGE(widgets->osc1.octave), patch->parameters[PARAMETER_OSC1_OCTAVE]);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.semitone), patch->parameters[PARAMETER_OSC1_SEMITONE]);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.fine), patch->parameters[PARAMETER_OSC1_FINE]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc1.wave), patch->parameters[PARAMETER_OSC1_WAVE]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc1.mod1_src), patch->parameters[PARAMETER_OSC1_MOD1_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.mod1_depth), patch->parameters[PARAMETER_OSC1_MOD1_DEPTH]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc1.mod2_src), patch->parameters[PARAMETER_OSC1_MOD2_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.mod2_depth), patch->parameters[PARAMETER_OSC1_MOD2_DEPTH]);

    printf("Setting DCA1 parameters: level %d, output %d, mod src1 %d, mod1 depth %d, mod2 src %d, mod2 depth %d\n",
        patch->parameters[PARAMETER_DCA1_LEVEL],
        patch->parameters[PARAMETER_DCA1_OUTPUT],
        patch->parameters[PARAMETER_DCA1_MOD1_SRC],
        patch->parameters[PARAMETER_DCA1_MOD1_DEPTH],
        patch->parameters[PARAMETER_DCA1_MOD2_SRC],
        patch->parameters[PARAMETER_DCA1_MOD2_DEPTH]);

    gtk_range_set_value(GTK_RANGE(widgets->osc1.dca_level), patch->parameters[PARAMETER_DCA1_LEVEL]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->osc1.dca_output), patch->parameters[PARAMETER_DCA1_OUTPUT] ? TRUE : FALSE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc1.dca_mod1_src), patch->parameters[PARAMETER_DCA1_MOD1_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.dca_mod1_depth), patch->parameters[PARAMETER_DCA1_MOD1_DEPTH]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc1.dca_mod2_src), patch->parameters[PARAMETER_DCA1_MOD2_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.dca_mod2_depth), patch->parameters[PARAMETER_DCA1_MOD2_DEPTH]);

    printf("Setting OSC2 parameters: octave %d, semitone %d, fine %d, wave %d, mod src1 %d, mod1 depth %d, mod2 src %d, mod2 depth %d\n",
        patch->parameters[PARAMETER_OSC2_OCTAVE],
        patch->parameters[PARAMETER_OSC2_SEMITONE],
        patch->parameters[PARAMETER_OSC2_FINE],
        patch->parameters[PARAMETER_OSC2_WAVE],
        patch->parameters[PARAMETER_OSC2_MOD1_SRC],
        patch->parameters[PARAMETER_OSC2_MOD1_DEPTH],
        patch->parameters[PARAMETER_OSC2_MOD2_SRC],
        patch->parameters[PARAMETER_OSC2_MOD2_DEPTH]);

    gtk_range_set_value(GTK_RANGE(widgets->osc2.octave), patch->parameters[PARAMETER_OSC2_OCTAVE]);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.semitone), patch->parameters[PARAMETER_OSC2_SEMITONE]);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.fine), patch->parameters[PARAMETER_OSC2_FINE]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc2.wave), patch->parameters[PARAMETER_OSC2_WAVE]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc2.mod1_src), patch->parameters[PARAMETER_OSC2_MOD1_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.mod1_depth), patch->parameters[PARAMETER_OSC2_MOD1_DEPTH]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc2.mod2_src), patch->parameters[PARAMETER_OSC2_MOD2_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.mod2_depth), patch->parameters[PARAMETER_OSC2_MOD2_DEPTH]);

    printf("Setting DCA2 parameters: level %d, output %d, mod src1 %d, mod1 depth %d, mod2 src %d, mod2 depth %d\n",
        patch->parameters[PARAMETER_DCA2_LEVEL],
        patch->parameters[PARAMETER_DCA2_OUTPUT],
        patch->parameters[PARAMETER_DCA2_MOD1_SRC],
        patch->parameters[PARAMETER_DCA2_MOD1_DEPTH],
        patch->parameters[PARAMETER_DCA2_MOD2_SRC],
        patch->parameters[PARAMETER_DCA2_MOD2_DEPTH]);

    gtk_range_set_value(GTK_RANGE(widgets->osc2.dca_level), patch->parameters[PARAMETER_DCA2_LEVEL]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->osc2.dca_output), patch->parameters[PARAMETER_DCA2_OUTPUT] ? TRUE : FALSE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc2.dca_mod1_src), patch->parameters[PARAMETER_DCA2_MOD1_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.dca_mod1_depth), patch->parameters[PARAMETER_DCA2_MOD1_DEPTH]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc2.dca_mod2_src), patch->parameters[PARAMETER_DCA2_MOD2_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.dca_mod2_depth), patch->parameters[PARAMETER_DCA2_MOD2_DEPTH]);

    printf("Setting OSC3 parameters: octave %d, semitone %d, fine %d, wave %d, mod src1 %d, mod1 depth %d, mod2 src %d, mod2 depth %d\n",
        patch->parameters[PARAMETER_OSC3_OCTAVE],
        patch->parameters[PARAMETER_OSC3_SEMITONE],
        patch->parameters[PARAMETER_OSC3_FINE],
        patch->parameters[PARAMETER_OSC3_WAVE],
        patch->parameters[PARAMETER_OSC3_MOD1_SRC],
        patch->parameters[PARAMETER_OSC3_MOD1_DEPTH],
        patch->parameters[PARAMETER_OSC3_MOD2_SRC],
        patch->parameters[PARAMETER_OSC3_MOD2_DEPTH]);

    gtk_range_set_value(GTK_RANGE(widgets->osc3.octave), patch->parameters[PARAMETER_OSC3_OCTAVE]);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.semitone), patch->parameters[PARAMETER_OSC3_SEMITONE]);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.fine), patch->parameters[PARAMETER_OSC3_FINE]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc3.wave), patch->parameters[PARAMETER_OSC3_WAVE]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc3.mod1_src), patch->parameters[PARAMETER_OSC3_MOD1_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.mod1_depth), patch->parameters[PARAMETER_OSC3_MOD1_DEPTH]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc3.mod2_src), patch->parameters[PARAMETER_OSC3_MOD2_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.mod2_depth), patch->parameters[PARAMETER_OSC3_MOD2_DEPTH]);

    printf("Setting DCA3 parameters: level %d, output %d, mod src1 %d, mod1 depth %d, mod2 src %d, mod2 depth %d\n",
        patch->parameters[PARAMETER_DCA3_LEVEL],
        patch->parameters[PARAMETER_DCA3_OUTPUT],
        patch->parameters[PARAMETER_DCA3_MOD1_SRC],
        patch->parameters[PARAMETER_DCA3_MOD1_DEPTH],
        patch->parameters[PARAMETER_DCA3_MOD2_SRC],
        patch->parameters[PARAMETER_DCA3_MOD2_DEPTH]);

    gtk_range_set_value(GTK_RANGE(widgets->osc3.dca_level), patch->parameters[PARAMETER_DCA3_LEVEL]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->osc3.dca_output), patch->parameters[PARAMETER_DCA3_OUTPUT] ? TRUE : FALSE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc3.dca_mod1_src), patch->parameters[PARAMETER_DCA3_MOD1_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.dca_mod1_depth), patch->parameters[PARAMETER_DCA3_MOD1_DEPTH]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc3.dca_mod2_src), patch->parameters[PARAMETER_DCA3_MOD2_SRC]);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.dca_mod2_depth), patch->parameters[PARAMETER_DCA3_MOD2_DEPTH]);
}

void
clear_oscillators_parameters(OscillatorsDialog *widgets)
{
    gtk_range_set_value(GTK_RANGE(widgets->osc1.octave), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.semitone), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.fine), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc1.wave), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc1.mod1_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.mod1_depth), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc1.mod2_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.mod2_depth), 0);

    gtk_range_set_value(GTK_RANGE(widgets->osc1.dca_level), 63);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->osc1.dca_output), TRUE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc1.dca_mod1_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.dca_mod1_depth), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc1.dca_mod2_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc1.dca_mod2_depth), 0);

    gtk_range_set_value(GTK_RANGE(widgets->osc2.octave), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.semitone), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.fine), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc2.wave), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc2.mod1_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.mod1_depth), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc2.mod2_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.mod2_depth), 0);

    gtk_range_set_value(GTK_RANGE(widgets->osc2.dca_level), 63);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->osc2.dca_output), TRUE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc2.dca_mod1_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.dca_mod1_depth), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc2.dca_mod2_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc2.dca_mod2_depth), 0);

    gtk_range_set_value(GTK_RANGE(widgets->osc3.octave), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.semitone), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.fine), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc3.wave), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc3.mod1_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.mod1_depth), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc3.mod2_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.mod2_depth), 0);

    gtk_range_set_value(GTK_RANGE(widgets->osc3.dca_level), 63);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->osc3.dca_output), TRUE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc3.dca_mod1_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.dca_mod1_depth), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->osc3.dca_mod2_src), 0);
    gtk_range_set_value(GTK_RANGE(widgets->osc3.dca_mod2_depth), 0);
}

static GtkWidget *
create_osc1(Oscillator *osc)
{
    GtkWidget *frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("Oscillator 1");

    grid = create_grid(GTK_CONTAINER(frame));

    label = gtk_label_new("Octave:");
    osc->octave = create_hscale(-3, 5, PARAMETER_OSC1_OCTAVE);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(osc->octave));

    label = gtk_label_new("Semitone:");
    osc->semitone = create_hscale(0, 11, PARAMETER_OSC1_SEMITONE);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(osc->semitone));

    label = gtk_label_new("Fine:");
    osc->fine = create_hscale(0, 31, PARAMETER_OSC1_FINE);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(osc->fine));

    label = gtk_label_new("Wave:");
    osc->wave = create_combo_box(waves, G_N_ELEMENTS(waves), PARAMETER_OSC1_WAVE);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(osc->wave));

    label = gtk_label_new("Mod 1:");
    osc->mod1_src = create_combo_box_with_entries(osc1_mod1_srcs, G_N_ELEMENTS(osc1_mod1_srcs));
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(osc->mod1_src));

    label = gtk_label_new("Mod 1 Depth:");
    osc->mod1_depth = create_hscale_with_params(-63, 63, &osc1_mod1_depth_params);
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(osc->mod1_depth));

    label = gtk_label_new("Mod 2:");
    osc->mod2_src = create_combo_box_with_entries(osc1_mod2_srcs, G_N_ELEMENTS(osc1_mod2_srcs));
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(osc->mod2_src));

    label = gtk_label_new("Mod 2 Depth:");
    osc->mod2_depth = create_hscale_with_params(-63, 63, &osc1_mod2_depth_params);
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(osc->mod2_depth));

    return frame;
}

static GtkWidget *
create_osc2(Oscillator *osc)
{
    GtkWidget *frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("Oscillator 2");

    grid = create_grid(GTK_CONTAINER(frame));

    label = gtk_label_new("Octave:");
    osc->octave = create_hscale(-3, 5, PARAMETER_OSC2_OCTAVE);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(osc->octave));

    label = gtk_label_new("Semitone:");
    osc->semitone = create_hscale(0, 11, PARAMETER_OSC2_SEMITONE);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(osc->semitone));

    label = gtk_label_new("Fine:");
    osc->fine = create_hscale(0, 31, PARAMETER_OSC2_FINE);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(osc->fine));

    label = gtk_label_new("Wave:");
    osc->wave = create_combo_box(waves, G_N_ELEMENTS(waves), PARAMETER_OSC2_WAVE);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(osc->wave));

    label = gtk_label_new("Mod 1:");
    osc->mod1_src = create_combo_box_with_entries(osc2_mod1_srcs, G_N_ELEMENTS(osc2_mod1_srcs));
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(osc->mod1_src));

    label = gtk_label_new("Mod 1 Depth:");
    osc->mod1_depth = create_hscale_with_params(-63, 63, &osc2_mod1_depth_params);
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(osc->mod1_depth));

    label = gtk_label_new("Mod 2:");
    osc->mod2_src = create_combo_box_with_entries(osc2_mod2_srcs, G_N_ELEMENTS(osc2_mod2_srcs));
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(osc->mod2_src));

    label = gtk_label_new("Mod 2 Depth:");
    osc->mod2_depth = create_hscale_with_params(-63, 63, &osc2_mod2_depth_params);
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(osc->mod2_depth));

    return frame;
}

static GtkWidget *
create_osc3(Oscillator *osc)
{
    GtkWidget *frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("Oscillator 3");

    grid = create_grid(GTK_CONTAINER(frame));

    label = gtk_label_new("Octave:");
    osc->octave = create_hscale(-3, 5, PARAMETER_OSC3_OCTAVE);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(osc->octave));

    label = gtk_label_new("Semitone:");
    osc->semitone = create_hscale(0, 11, PARAMETER_OSC3_SEMITONE);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(osc->semitone));

    label = gtk_label_new("Fine:");
    osc->fine = create_hscale(0, 31, PARAMETER_OSC3_FINE);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(osc->fine));

    label = gtk_label_new("Wave:");
    osc->wave = create_combo_box(waves, G_N_ELEMENTS(waves), PARAMETER_OSC3_WAVE);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(osc->wave));

    label = gtk_label_new("Mod 1:");
    osc->mod1_src = create_combo_box_with_entries(osc3_mod1_srcs, G_N_ELEMENTS(osc3_mod1_srcs));
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(osc->mod1_src));

    label = gtk_label_new("Mod 1 Depth:");
    osc->mod1_depth = create_hscale_with_params(-63, 63, &osc3_mod1_depth_params);
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(osc->mod1_depth));

    label = gtk_label_new("Mod 2:");
    osc->mod2_src = create_combo_box_with_entries(osc3_mod2_srcs, G_N_ELEMENTS(osc3_mod2_srcs));
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(osc->mod2_src));

    label = gtk_label_new("Mod 2 Depth:");
    osc->mod2_depth = create_hscale_with_params(-63, 63, &osc3_mod2_depth_params);
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(osc->mod2_depth));

    return frame;
}

static GtkWidget *
create_dca1(Oscillator *osc)
{
    GtkWidget *frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("DCA 1");

    grid = create_grid(GTK_CONTAINER(frame));

    label = gtk_label_new("Level:");
    osc->dca_level = create_hscale(0, 63, PARAMETER_DCA1_LEVEL);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(osc->dca_level));

    label = gtk_label_new("Output:");
    osc->dca_output = create_check_button(PARAMETER_DCA1_OUTPUT);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(osc->dca_output));

    label = gtk_label_new("Mod 1:");
    osc->dca_mod1_src = create_combo_box_with_entries(dca1_mod1_srcs, G_N_ELEMENTS(dca1_mod1_srcs));
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod1_src));

    label = gtk_label_new("Mod 1 Depth:");
    osc->dca_mod1_depth = create_hscale_with_params(-63, 63, &dca1_mod1_depth_params);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod1_depth));

    label = gtk_label_new("Mod 2:");
    osc->dca_mod2_src = create_combo_box_with_entries(dca1_mod2_srcs, G_N_ELEMENTS(dca1_mod2_srcs));
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod2_src));

    label = gtk_label_new("Mod 2 Depth:");
    osc->dca_mod2_depth = create_hscale_with_params(-63, 63, &dca1_mod2_depth_params);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod2_depth));

    return frame;
}

static GtkWidget *
create_dca2(Oscillator *osc)
{
    GtkWidget *frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("DCA 1");

    grid = create_grid(GTK_CONTAINER(frame));

    label = gtk_label_new("Level:");
    osc->dca_level = create_hscale(0, 63, PARAMETER_DCA2_LEVEL);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(osc->dca_level));

    label = gtk_label_new("Output:");
    osc->dca_output = create_check_button(PARAMETER_DCA2_OUTPUT);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(osc->dca_output));

    label = gtk_label_new("Mod 1:");
    osc->dca_mod1_src = create_combo_box_with_entries(dca2_mod1_srcs, G_N_ELEMENTS(dca2_mod1_srcs));
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod1_src));

    label = gtk_label_new("Mod 1 Depth:");
    osc->dca_mod1_depth = create_hscale_with_params(-63, 63, &dca2_mod1_depth_params);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod1_depth));

    label = gtk_label_new("Mod 2:");
    osc->dca_mod2_src = create_combo_box_with_entries(dca2_mod2_srcs, G_N_ELEMENTS(dca2_mod2_srcs));
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod2_src));

    label = gtk_label_new("Mod 2 Depth:");
    osc->dca_mod2_depth = create_hscale_with_params(-63, 63, &dca2_mod2_depth_params);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod2_depth));

    return frame;
}

static GtkWidget *
create_dca3(Oscillator *osc)
{
    GtkWidget *frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("DCA 3");

    grid = create_grid(GTK_CONTAINER(frame));

    label = gtk_label_new("Level:");
    osc->dca_level = create_hscale(0, 63, PARAMETER_DCA3_LEVEL);
    create_grid_row(grid, 0, GTK_LABEL(label), GTK_WIDGET(osc->dca_level));

    label = gtk_label_new("Output:");
    osc->dca_output = create_check_button(PARAMETER_DCA3_OUTPUT);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(osc->dca_output));

    label = gtk_label_new("Mod 1:");
    osc->dca_mod1_src = create_combo_box_with_entries(dca3_mod1_srcs, G_N_ELEMENTS(dca3_mod1_srcs));
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod1_src));

    label = gtk_label_new("Mod 1 Depth:");
    osc->dca_mod1_depth = create_hscale_with_params(-63, 63, &dca3_mod1_depth_params);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod1_depth));

    label = gtk_label_new("Mod 2:");
    osc->dca_mod2_src = create_combo_box_with_entries(dca3_mod2_srcs, G_N_ELEMENTS(dca3_mod2_srcs));
    create_grid_row(grid, 5, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod2_src));

    label = gtk_label_new("Mod 2 Depth:");
    osc->dca_mod2_depth = create_hscale_with_params(-63, 63, &dca3_mod2_depth_params);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(osc->dca_mod2_depth));

    return frame;
}
