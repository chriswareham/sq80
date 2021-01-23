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
#include "envelopes.h"

static ScaleParams env1_level1_params = {
    .parameter = 0x00,
    .offset = 64
};

static ScaleParams env1_level2_params = {
    .parameter = 0x01,
    .offset = 64
};

static ScaleParams env1_level3_params = {
    .parameter = 0x02,
    .offset = 64
};

static ScaleParams env1_velocity_attack_params = {
    .parameter = 0x04,
    .multiplier = 2
};

static ScaleParams env1_time1_params = {
    .parameter = 0x05,
    .multiplier = 2
};

static ScaleParams env1_time2_params = {
    .parameter = 0x06,
    .multiplier = 2
};

static ScaleParams env1_time3_params = {
    .parameter = 0x07,
    .multiplier = 2
};

static ScaleParams env1_keyboard_decay_scaling_params = {
    .parameter = 0x09,
    .multiplier = 2
};

static ScaleParams env2_level1_params = {
    .parameter = 0x0a,
    .offset = 64
};

static ScaleParams env2_level2_params = {
    .parameter = 0x0b,
    .offset = 64
};

static ScaleParams env2_level3_params = {
    .parameter = 0x0c,
    .offset = 64
};

static ScaleParams env2_velocity_attack_params = {
    .parameter = 0x0e,
    .multiplier = 2
};

static ScaleParams env2_time1_params = {
    .parameter = 0x0f,
    .multiplier = 2
};

static ScaleParams env2_time2_params = {
    .parameter = 0x10,
    .multiplier = 2
};

static ScaleParams env2_time3_params = {
    .parameter = 0x11,
    .multiplier = 2
};

static ScaleParams env2_keyboard_decay_scaling_params = {
    .parameter = 0x13,
    .multiplier = 2
};

static ScaleParams env3_level1_params = {
    .parameter = 0x14,
    .offset = 64
};

static ScaleParams env3_level2_params = {
    .parameter = 0x15,
    .offset = 64
};

static ScaleParams env3_velocity_attack_params = {
    .parameter = 0x18,
    .multiplier = 2
};

static ScaleParams env3_time1_params = {
    .parameter = 0x19,
    .multiplier = 2
};

static ScaleParams env3_time2_params = {
    .parameter = 0x1a,
    .multiplier = 2
};

static ScaleParams env3_time3_params = {
    .parameter = 0x1b,
    .multiplier = 2
};

static ScaleParams env3_level3_params = {
    .parameter = 0x16,
    .offset = 64
};

static ScaleParams env3_keyboard_decay_scaling_params = {
    .parameter = 0x1d,
    .multiplier = 2
};

static ScaleParams env4_level1_params = {
    .parameter = 0x1e,
    .offset = 64
};

static ScaleParams env4_level2_params = {
    .parameter = 0x1f,
    .offset = 64
};

static ScaleParams env4_level3_params = {
    .parameter = 0x20,
    .offset = 64
};

static ScaleParams env4_velocity_attack_params = {
    .parameter = 0x22,
    .multiplier = 2
};

static ScaleParams env4_time1_params = {
    .parameter = 0x23,
    .multiplier = 2
};

static ScaleParams env4_time2_params = {
    .parameter = 0x24,
    .multiplier = 2
};

static ScaleParams env4_time3_params = {
    .parameter = 0x25,
    .multiplier = 2
};

static ScaleParams env4_keyboard_decay_scaling_params = {
    .parameter = 0x27,
    .multiplier = 2
};

static GtkWidget *create_envelope1(Envelope *);
static GtkWidget *create_envelope2(Envelope *);
static GtkWidget *create_envelope3(Envelope *);
static GtkWidget *create_envelope4(Envelope *);
static gboolean envelope_event_callback(GtkWidget *, cairo_t *, gpointer);
static void envelope_callback(GtkWidget *, gpointer);
static void envelope_draw(Envelope *);

EnvelopesDialog *
new_envelopes_dialog(GtkWindow *parent)
{
    EnvelopesDialog *widgets;
    GtkGrid *grid;
    GtkWidget *button_box, *button;

    widgets = g_new(EnvelopesDialog, 1);

    widgets->dialog = create_window(parent, "Envelopes", FALSE);

    widgets->env1.dialog = widgets->dialog;
    widgets->env2.dialog = widgets->dialog;
    widgets->env3.dialog = widgets->dialog;
    widgets->env4.dialog = widgets->dialog;

    grid = create_grid(GTK_CONTAINER(widgets->dialog));

    gtk_grid_attach(grid, create_envelope1(&widgets->env1), 0, 0, 1, 1);
    gtk_grid_attach(grid, create_envelope2(&widgets->env2), 1, 0, 1, 1);
    gtk_grid_attach(grid, create_envelope3(&widgets->env3), 2, 0, 1, 1);
    gtk_grid_attach(grid, create_envelope4(&widgets->env4), 3, 0, 1, 1);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 6);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_grid_attach(grid, button_box, 0, 1, 4, 1);

    button = gtk_button_new_with_label("Close");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(close_window_callback), widgets->dialog);
    gtk_container_add(GTK_CONTAINER(button_box), button);

    return widgets;
}

void
show_envelopes_dialog(EnvelopesDialog *widgets)
{
    show_window(widgets->dialog);
}

void
set_envelopes_parameters(EnvelopesDialog *widgets, Patch *patch)
{
    printf("Setting env 1 parameters: level 1 %d, level 2 %d, level 3 %d, vel level %d, vel attack %d, time 1 %d, time 2 %d, time 3 %d, time 4 %d, key scaling %d\n",
        patch->parameters[PARAMETER_ENV1_LEVEL1],
        patch->parameters[PARAMETER_ENV1_LEVEL2],
        patch->parameters[PARAMETER_ENV1_LEVEL3],
        patch->parameters[PARAMETER_ENV1_VELOCITY_LEVEL],
        patch->parameters[PARAMETER_ENV1_VELOCITY_ATTACK],
        patch->parameters[PARAMETER_ENV1_TIME1],
        patch->parameters[PARAMETER_ENV1_TIME2],
        patch->parameters[PARAMETER_ENV1_TIME3],
        patch->parameters[PARAMETER_ENV1_TIME4],
        patch->parameters[PARAMETER_ENV1_KEYBOARD_DECAY_SCALING]);

    gtk_range_set_value(GTK_RANGE(widgets->env1.level1), patch->parameters[PARAMETER_ENV1_LEVEL1]);
    gtk_range_set_value(GTK_RANGE(widgets->env1.level2), patch->parameters[PARAMETER_ENV1_LEVEL2]);
    gtk_range_set_value(GTK_RANGE(widgets->env1.level3), patch->parameters[PARAMETER_ENV1_LEVEL3]);
    gtk_range_set_value(GTK_RANGE(widgets->env1.velocity_level), patch->parameters[PARAMETER_ENV1_VELOCITY_LEVEL]);
    gtk_range_set_value(GTK_RANGE(widgets->env1.velocity_attack), patch->parameters[PARAMETER_ENV1_VELOCITY_ATTACK]);
    gtk_range_set_value(GTK_RANGE(widgets->env1.time1), patch->parameters[PARAMETER_ENV1_TIME1]);
    gtk_range_set_value(GTK_RANGE(widgets->env1.time2), patch->parameters[PARAMETER_ENV1_TIME2]);
    gtk_range_set_value(GTK_RANGE(widgets->env1.time3), patch->parameters[PARAMETER_ENV1_TIME3]);
    gtk_range_set_value(GTK_RANGE(widgets->env1.time4), patch->parameters[PARAMETER_ENV1_TIME4]);
    gtk_range_set_value(GTK_RANGE(widgets->env1.keyboard_decay_scaling), patch->parameters[PARAMETER_ENV1_KEYBOARD_DECAY_SCALING]);

    printf("Setting env 2 parameters: level 1 %d, level 2 %d, level 3 %d, vel level %d, vel attack %d, time 1 %d, time 2 %d, time 3 %d, time 4 %d, key scaling %d\n",
        patch->parameters[PARAMETER_ENV2_LEVEL1],
        patch->parameters[PARAMETER_ENV2_LEVEL2],
        patch->parameters[PARAMETER_ENV2_LEVEL3],
        patch->parameters[PARAMETER_ENV2_VELOCITY_LEVEL],
        patch->parameters[PARAMETER_ENV2_VELOCITY_ATTACK],
        patch->parameters[PARAMETER_ENV2_TIME1],
        patch->parameters[PARAMETER_ENV2_TIME2],
        patch->parameters[PARAMETER_ENV2_TIME3],
        patch->parameters[PARAMETER_ENV2_TIME4],
        patch->parameters[PARAMETER_ENV2_KEYBOARD_DECAY_SCALING]);

    gtk_range_set_value(GTK_RANGE(widgets->env2.level1), patch->parameters[PARAMETER_ENV2_LEVEL1]);
    gtk_range_set_value(GTK_RANGE(widgets->env2.level2), patch->parameters[PARAMETER_ENV2_LEVEL2]);
    gtk_range_set_value(GTK_RANGE(widgets->env2.level3), patch->parameters[PARAMETER_ENV2_LEVEL3]);
    gtk_range_set_value(GTK_RANGE(widgets->env2.velocity_level), patch->parameters[PARAMETER_ENV2_VELOCITY_LEVEL]);
    gtk_range_set_value(GTK_RANGE(widgets->env2.velocity_attack), patch->parameters[PARAMETER_ENV2_VELOCITY_ATTACK]);
    gtk_range_set_value(GTK_RANGE(widgets->env2.time1), patch->parameters[PARAMETER_ENV2_TIME1]);
    gtk_range_set_value(GTK_RANGE(widgets->env2.time2), patch->parameters[PARAMETER_ENV2_TIME2]);
    gtk_range_set_value(GTK_RANGE(widgets->env2.time3), patch->parameters[PARAMETER_ENV2_TIME3]);
    gtk_range_set_value(GTK_RANGE(widgets->env2.time4), patch->parameters[PARAMETER_ENV2_TIME4]);
    gtk_range_set_value(GTK_RANGE(widgets->env2.keyboard_decay_scaling), patch->parameters[PARAMETER_ENV2_KEYBOARD_DECAY_SCALING]);

    printf("Setting env 3 parameters: level 1 %d, level 2 %d, level 3 %d, vel level %d, vel attack %d, time 1 %d, time 2 %d, time 3 %d, time 4 %d, key scaling %d\n",
        patch->parameters[PARAMETER_ENV3_LEVEL1],
        patch->parameters[PARAMETER_ENV3_LEVEL2],
        patch->parameters[PARAMETER_ENV3_LEVEL3],
        patch->parameters[PARAMETER_ENV3_VELOCITY_LEVEL],
        patch->parameters[PARAMETER_ENV3_VELOCITY_ATTACK],
        patch->parameters[PARAMETER_ENV3_TIME1],
        patch->parameters[PARAMETER_ENV3_TIME2],
        patch->parameters[PARAMETER_ENV3_TIME3],
        patch->parameters[PARAMETER_ENV3_TIME4],
        patch->parameters[PARAMETER_ENV3_KEYBOARD_DECAY_SCALING]);

    gtk_range_set_value(GTK_RANGE(widgets->env3.level1), patch->parameters[PARAMETER_ENV3_LEVEL1]);
    gtk_range_set_value(GTK_RANGE(widgets->env3.level2), patch->parameters[PARAMETER_ENV3_LEVEL2]);
    gtk_range_set_value(GTK_RANGE(widgets->env3.level3), patch->parameters[PARAMETER_ENV3_LEVEL3]);
    gtk_range_set_value(GTK_RANGE(widgets->env3.velocity_level), patch->parameters[PARAMETER_ENV3_VELOCITY_LEVEL]);
    gtk_range_set_value(GTK_RANGE(widgets->env3.velocity_attack), patch->parameters[PARAMETER_ENV3_VELOCITY_ATTACK]);
    gtk_range_set_value(GTK_RANGE(widgets->env3.time1), patch->parameters[PARAMETER_ENV3_TIME1]);
    gtk_range_set_value(GTK_RANGE(widgets->env3.time2), patch->parameters[PARAMETER_ENV3_TIME2]);
    gtk_range_set_value(GTK_RANGE(widgets->env3.time3), patch->parameters[PARAMETER_ENV3_TIME3]);
    gtk_range_set_value(GTK_RANGE(widgets->env3.time4), patch->parameters[PARAMETER_ENV3_TIME4]);
    gtk_range_set_value(GTK_RANGE(widgets->env3.keyboard_decay_scaling), patch->parameters[PARAMETER_ENV3_KEYBOARD_DECAY_SCALING]);

    printf("Setting env 4 parameters: level 1 %d, level 2 %d, level 3 %d, vel level %d, vel attack %d, time 1 %d, time 2 %d, time 3 %d, time 4 %d, key scaling %d\n",
        patch->parameters[PARAMETER_ENV4_LEVEL1],
        patch->parameters[PARAMETER_ENV4_LEVEL2],
        patch->parameters[PARAMETER_ENV4_LEVEL3],
        patch->parameters[PARAMETER_ENV4_VELOCITY_LEVEL],
        patch->parameters[PARAMETER_ENV4_VELOCITY_ATTACK],
        patch->parameters[PARAMETER_ENV4_TIME1],
        patch->parameters[PARAMETER_ENV4_TIME2],
        patch->parameters[PARAMETER_ENV4_TIME3],
        patch->parameters[PARAMETER_ENV4_TIME4],
        patch->parameters[PARAMETER_ENV4_KEYBOARD_DECAY_SCALING]);

    gtk_range_set_value(GTK_RANGE(widgets->env4.level1), patch->parameters[PARAMETER_ENV4_LEVEL1]);
    gtk_range_set_value(GTK_RANGE(widgets->env4.level2), patch->parameters[PARAMETER_ENV4_LEVEL2]);
    gtk_range_set_value(GTK_RANGE(widgets->env4.level3), patch->parameters[PARAMETER_ENV4_LEVEL3]);
    gtk_range_set_value(GTK_RANGE(widgets->env4.velocity_level), patch->parameters[PARAMETER_ENV4_VELOCITY_LEVEL]);
    gtk_range_set_value(GTK_RANGE(widgets->env4.velocity_attack), patch->parameters[PARAMETER_ENV4_VELOCITY_ATTACK]);
    gtk_range_set_value(GTK_RANGE(widgets->env4.time1), patch->parameters[PARAMETER_ENV4_TIME1]);
    gtk_range_set_value(GTK_RANGE(widgets->env4.time2), patch->parameters[PARAMETER_ENV4_TIME2]);
    gtk_range_set_value(GTK_RANGE(widgets->env4.time3), patch->parameters[PARAMETER_ENV4_TIME3]);
    gtk_range_set_value(GTK_RANGE(widgets->env4.time4), patch->parameters[PARAMETER_ENV4_TIME4]);
    gtk_range_set_value(GTK_RANGE(widgets->env4.keyboard_decay_scaling), patch->parameters[PARAMETER_ENV4_KEYBOARD_DECAY_SCALING]);
}

void
clear_envelopes_parameters(EnvelopesDialog *widgets)
{
    gtk_range_set_value(GTK_RANGE(widgets->env1.level1), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env1.level2), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env1.level3), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env1.velocity_level), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env1.velocity_attack), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env1.time1), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env1.time2), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env1.time3), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env1.time4), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env1.keyboard_decay_scaling), 0);

    gtk_range_set_value(GTK_RANGE(widgets->env2.level1), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env2.level2), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env2.level3), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env2.velocity_level), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env2.velocity_attack), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env2.time1), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env2.time2), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env2.time3), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env2.time4), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env2.keyboard_decay_scaling), 0);

    gtk_range_set_value(GTK_RANGE(widgets->env3.level1), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env3.level2), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env3.level3), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env3.velocity_level), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env3.velocity_attack), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env3.time1), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env3.time2), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env3.time3), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env3.time4), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env3.keyboard_decay_scaling), 0);

    gtk_range_set_value(GTK_RANGE(widgets->env4.level1), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env4.level2), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env4.level3), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env4.velocity_level), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env4.velocity_attack), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env4.time1), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env4.time2), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env4.time3), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env4.time4), 0);
    gtk_range_set_value(GTK_RANGE(widgets->env4.keyboard_decay_scaling), 0);
}

static GtkWidget *
create_envelope1(Envelope *env)
{
    GtkWidget *frame, *envelope_frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("Envelope 1");

    grid = create_grid(GTK_CONTAINER(frame));

    envelope_frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(envelope_frame), GTK_SHADOW_IN);
    gtk_grid_attach(grid, envelope_frame, 0, 0, 2, 1);

    env->envelope = gtk_drawing_area_new();
    gtk_widget_set_size_request(GTK_WIDGET(env->envelope), 317, 127);
    g_signal_connect(G_OBJECT(env->envelope), "draw", G_CALLBACK(envelope_event_callback), env);
    gtk_container_add(GTK_CONTAINER(envelope_frame), env->envelope);

    label = gtk_label_new("Level 1:");
    env->level1 = create_hscale_with_params(-63, 63, &env1_level1_params);
    g_signal_connect(G_OBJECT(env->level1), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(env->level1));

    label = gtk_label_new("Level 2:");
    env->level2 = create_hscale_with_params(-63, 63, &env1_level2_params);
    g_signal_connect(G_OBJECT(env->level2), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(env->level2));

    label = gtk_label_new("Level 3:");
    env->level3 = create_hscale_with_params(-63, 63, &env1_level3_params);
    g_signal_connect(G_OBJECT(env->level3), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(env->level3));

    label = gtk_label_new("Velocity Level:");
    env->velocity_level = create_hscale(0, 127, 0x03);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(env->velocity_level));

    label = gtk_label_new("(0 - 63 linear, 64 - 127 exponential)");
    gtk_grid_attach(grid, label, 0, 5, 2, 1);

    label = gtk_label_new("Velocity Attack:");
    env->velocity_attack = create_hscale_with_params(0, 63, &env1_velocity_attack_params);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(env->velocity_attack));

    label = gtk_label_new("Time 1:");
    env->time1 = create_hscale_with_params(0, 63, &env1_time1_params);
    g_signal_connect(G_OBJECT(env->time1), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(env->time1));

    label = gtk_label_new("Time 2:");
    env->time2 = create_hscale_with_params(0, 63, &env1_time2_params);
    g_signal_connect(G_OBJECT(env->time2), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 8, GTK_LABEL(label), GTK_WIDGET(env->time2));

    label = gtk_label_new("Time 3:");
    env->time3 = create_hscale_with_params(0, 63, &env1_time3_params);
    g_signal_connect(G_OBJECT(env->time3), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 9, GTK_LABEL(label), GTK_WIDGET(env->time3));

    label = gtk_label_new("Time 4:");
    env->time4 = create_hscale(0, 127, 0x08);
    g_signal_connect(G_OBJECT(env->time4), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 10, GTK_LABEL(label), GTK_WIDGET(env->time4));

    label = gtk_label_new("(0 - 63 normal, 64 - 127 repeat)");
    gtk_grid_attach(grid, label, 0, 11, 2, 1);

    label = gtk_label_new("Keyboard Decay Scaling:");
    env->keyboard_decay_scaling = create_hscale_with_params(0, 63, &env1_keyboard_decay_scaling_params);
    create_grid_row(grid, 12, GTK_LABEL(label), GTK_WIDGET(env->keyboard_decay_scaling));

    return frame;
}

static GtkWidget *
create_envelope2(Envelope *env)
{
    GtkWidget *frame, *envelope_frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("Envelope 2");

    grid = create_grid(GTK_CONTAINER(frame));

    envelope_frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(envelope_frame), GTK_SHADOW_IN);
    gtk_grid_attach(grid, envelope_frame, 0, 0, 2, 1);

    env->envelope = gtk_drawing_area_new();
    gtk_widget_set_size_request(GTK_WIDGET(env->envelope), 317, 127);
    g_signal_connect(G_OBJECT(env->envelope), "draw", G_CALLBACK(envelope_event_callback), env);
    gtk_container_add(GTK_CONTAINER(envelope_frame), env->envelope);

    label = gtk_label_new("Level 1:");
    env->level1 = create_hscale_with_params(-63, 63, &env2_level1_params);
    g_signal_connect(G_OBJECT(env->level1), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(env->level1));

    label = gtk_label_new("Level 2:");
    env->level2 = create_hscale_with_params(-63, 63, &env2_level2_params);
    g_signal_connect(G_OBJECT(env->level2), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(env->level2));

    label = gtk_label_new("Level 3:");
    env->level3 = create_hscale_with_params(-63, 63, &env2_level3_params);
    g_signal_connect(G_OBJECT(env->level3), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(env->level3));

    label = gtk_label_new("Velocity Level:");
    env->velocity_level = create_hscale(0, 127, 0x0d);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(env->velocity_level));

    label = gtk_label_new("(0 - 63 linear, 64 - 127 exponential)");
    gtk_grid_attach(grid, label, 0, 5, 2, 1);

    label = gtk_label_new("Velocity Attack:");
    env->velocity_attack = create_hscale_with_params(0, 63, &env2_velocity_attack_params);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(env->velocity_attack));

    label = gtk_label_new("Time 1:");
    env->time1 = create_hscale_with_params(0, 63, &env2_time1_params);
    g_signal_connect(G_OBJECT(env->time1), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(env->time1));

    label = gtk_label_new("Time 2:");
    env->time2 = create_hscale_with_params(0, 63, &env2_time2_params);
    g_signal_connect(G_OBJECT(env->time2), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 8, GTK_LABEL(label), GTK_WIDGET(env->time2));

    label = gtk_label_new("Time 3:");
    env->time3 = create_hscale_with_params(0, 63, &env2_time3_params);
    g_signal_connect(G_OBJECT(env->time3), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 9, GTK_LABEL(label), GTK_WIDGET(env->time3));

    label = gtk_label_new("Time 4:");
    env->time4 = create_hscale(0, 127, 0x12);
    g_signal_connect(G_OBJECT(env->time4), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 10, GTK_LABEL(label), GTK_WIDGET(env->time4));

    label = gtk_label_new("(0 - 63 normal, 64 - 127 repeat)");
    gtk_grid_attach(grid, label, 0, 11, 2, 1);

    label = gtk_label_new("Keyboard Decay Scaling:");
    env->keyboard_decay_scaling = create_hscale_with_params(0, 63, &env2_keyboard_decay_scaling_params);
    create_grid_row(grid, 12, GTK_LABEL(label), GTK_WIDGET(env->keyboard_decay_scaling));

    return frame;
}

static GtkWidget *
create_envelope3(Envelope *env)
{
    GtkWidget *frame, *envelope_frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("Envelope 3");

    grid = create_grid(GTK_CONTAINER(frame));

    envelope_frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(envelope_frame), GTK_SHADOW_IN);
    gtk_grid_attach(grid, envelope_frame, 0, 0, 2, 1);

    env->envelope = gtk_drawing_area_new();
    gtk_widget_set_size_request(GTK_WIDGET(env->envelope), 317, 127);
    g_signal_connect(G_OBJECT(env->envelope), "draw", G_CALLBACK(envelope_event_callback), env);
    gtk_container_add(GTK_CONTAINER(envelope_frame), env->envelope);

    label = gtk_label_new("Level 1:");
    env->level1 = create_hscale_with_params(-63, 63, &env3_level1_params);
    g_signal_connect(G_OBJECT(env->level1), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(env->level1));

    label = gtk_label_new("Level 2:");
    env->level2 = create_hscale_with_params(-63, 63, &env3_level2_params);
    g_signal_connect(G_OBJECT(env->level2), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(env->level2));

    label = gtk_label_new("Level 3:");
    env->level3 = create_hscale_with_params(-63, 63, &env3_level3_params);
    g_signal_connect(G_OBJECT(env->level3), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(env->level3));

    label = gtk_label_new("Velocity Level:");
    env->velocity_level = create_hscale(0, 127, 0x17);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(env->velocity_level));

    label = gtk_label_new("(0 - 63 linear, 64 - 127 exponential)");
    gtk_grid_attach(grid, label, 0, 5, 2, 1);

    label = gtk_label_new("Velocity Attack:");
    env->velocity_attack = create_hscale_with_params(0, 63, &env3_velocity_attack_params);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(env->velocity_attack));

    label = gtk_label_new("Time 1:");
    env->time1 = create_hscale_with_params(0, 63, &env3_time1_params);
    g_signal_connect(G_OBJECT(env->time1), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(env->time1));

    label = gtk_label_new("Time 2:");
    env->time2 = create_hscale_with_params(0, 63, &env3_time2_params);
    g_signal_connect(G_OBJECT(env->time2), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 8, GTK_LABEL(label), GTK_WIDGET(env->time2));

    label = gtk_label_new("Time 3:");
    env->time3 = create_hscale_with_params(0, 63, &env3_time3_params);
    g_signal_connect(G_OBJECT(env->time3), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 9, GTK_LABEL(label), GTK_WIDGET(env->time3));

    label = gtk_label_new("Time 4:");
    env->time4 = create_hscale(0, 127, 0x1c);
    g_signal_connect(G_OBJECT(env->time4), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 10, GTK_LABEL(label), GTK_WIDGET(env->time4));

    label = gtk_label_new("(0 - 63 normal, 64 - 127 repeat)");
    gtk_grid_attach(grid, label, 0, 11, 2, 1);

    label = gtk_label_new("Keyboard Decay Scaling:");
    env->keyboard_decay_scaling = create_hscale_with_params(0, 63, &env3_keyboard_decay_scaling_params);
    create_grid_row(grid, 12, GTK_LABEL(label), GTK_WIDGET(env->keyboard_decay_scaling));

    return frame;
}

static GtkWidget *
create_envelope4(Envelope *env)
{
    GtkWidget *frame, *envelope_frame, *label;
    GtkGrid *grid;

    frame = gtk_frame_new("Envelope 4");

    grid = create_grid(GTK_CONTAINER(frame));

    envelope_frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(envelope_frame), GTK_SHADOW_IN);
    gtk_grid_attach(grid, envelope_frame, 0, 0, 2, 1);

    env->envelope = gtk_drawing_area_new();
    gtk_widget_set_size_request(GTK_WIDGET(env->envelope), 317, 127);
    g_signal_connect(G_OBJECT(env->envelope), "draw", G_CALLBACK(envelope_event_callback), env);
    gtk_container_add(GTK_CONTAINER(envelope_frame), env->envelope);

    label = gtk_label_new("Level 1:");
    env->level1 = create_hscale_with_params(-63, 63, &env4_level1_params);
    g_signal_connect(G_OBJECT(env->level1), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 1, GTK_LABEL(label), GTK_WIDGET(env->level1));

    label = gtk_label_new("Level 2:");
    env->level2 = create_hscale_with_params(-63, 63, &env4_level2_params);
    g_signal_connect(G_OBJECT(env->level2), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 2, GTK_LABEL(label), GTK_WIDGET(env->level2));

    label = gtk_label_new("Level 3:");
    env->level3 = create_hscale_with_params(-63, 63, &env4_level3_params);
    g_signal_connect(G_OBJECT(env->level3), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 3, GTK_LABEL(label), GTK_WIDGET(env->level3));

    label = gtk_label_new("Velocity Level:");
    env->velocity_level = create_hscale(0, 127, 0x21);
    create_grid_row(grid, 4, GTK_LABEL(label), GTK_WIDGET(env->velocity_level));

    label = gtk_label_new("(0 - 63 linear, 64 - 127 exponential)");
    gtk_grid_attach(grid, label, 0, 5, 2, 1);

    label = gtk_label_new("Velocity Attack:");
    env->velocity_attack = create_hscale_with_params(0, 63, &env4_velocity_attack_params);
    create_grid_row(grid, 6, GTK_LABEL(label), GTK_WIDGET(env->velocity_attack));

    label = gtk_label_new("Time 1:");
    env->time1 = create_hscale_with_params(0, 63, &env4_time1_params);
    g_signal_connect(G_OBJECT(env->time1), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 7, GTK_LABEL(label), GTK_WIDGET(env->time1));

    label = gtk_label_new("Time 2:");
    env->time2 = create_hscale_with_params(0, 63, &env4_time2_params);
    g_signal_connect(G_OBJECT(env->time2), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 8, GTK_LABEL(label), GTK_WIDGET(env->time2));

    label = gtk_label_new("Time 3:");
    env->time3 = create_hscale_with_params(0, 63, &env4_time3_params);
    g_signal_connect(G_OBJECT(env->time3), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 9, GTK_LABEL(label), GTK_WIDGET(env->time3));

    label = gtk_label_new("Time 4:");
    env->time4 = create_hscale(0, 127, 0x26);
    g_signal_connect(G_OBJECT(env->time4), "value-changed", G_CALLBACK(envelope_callback), env);
    create_grid_row(grid, 10, GTK_LABEL(label), GTK_WIDGET(env->time4));

    label = gtk_label_new("(0 - 63 normal, 64 - 127 repeat)");
    gtk_grid_attach(grid, label, 0, 11, 2, 1);

    label = gtk_label_new("Keyboard Decay Scaling:");
    env->keyboard_decay_scaling = create_hscale_with_params(0, 63, &env4_keyboard_decay_scaling_params);
    create_grid_row(grid, 12, GTK_LABEL(label), GTK_WIDGET(env->keyboard_decay_scaling));

    return frame;
}
static gboolean
envelope_event_callback(GtkWidget *widget, cairo_t *cairo, gpointer data)
{
    Envelope *env = (Envelope *) data;

    envelope_draw(env);

    return TRUE;
}

static void
envelope_callback(GtkWidget *widget, gpointer data)
{
    Envelope *env = (Envelope *) data;

    if (gtk_widget_get_visible(GTK_WIDGET(env->dialog))) {
        envelope_draw(env);
    }
}

static void
envelope_draw(Envelope *widgets)
{
    GtkAllocation allocation;
    GdkWindow *window;
    GdkDrawingContext *drawing_context;
    cairo_region_t *cairo_region;
    cairo_t *cairo;
    gint t1, l1, t2, l2, t3, l3, t4;
    double dashes[2];

    gtk_widget_get_allocation(widgets->envelope, &allocation);

    t1 = gtk_range_get_value(GTK_RANGE(widgets->time1)) / 2;
    l1 = gtk_range_get_value(GTK_RANGE(widgets->level1));
    t2 = gtk_range_get_value(GTK_RANGE(widgets->time2)) / 2 + t1;
    l2 = gtk_range_get_value(GTK_RANGE(widgets->level2));
    t3 = gtk_range_get_value(GTK_RANGE(widgets->time3)) / 2 + t2;
    l3 = gtk_range_get_value(GTK_RANGE(widgets->level3));
    t4 = gtk_range_get_value(GTK_RANGE(widgets->time4)) / 2;

    dashes[0] = 2;
    dashes[1] = 2;

    /* create graphics context */

    window = gtk_widget_get_window(widgets->envelope);

    cairo_region = cairo_region_create();

    drawing_context = gdk_window_begin_draw_frame(window, cairo_region);

    cairo = gdk_drawing_context_get_cairo_context(drawing_context);

    /* draw background */

    cairo_set_source_rgb(cairo, 255, 255, 255);
    cairo_rectangle(cairo, 0, 0, allocation.width, allocation.height);
    cairo_fill(cairo);

    /* draw envelope */

    cairo_set_source_rgb(cairo, 0, 0, 0);

    cairo_set_line_width(cairo, 2);
    cairo_set_line_cap(cairo, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cairo, CAIRO_LINE_JOIN_ROUND);

    cairo_move_to(cairo, 0, allocation.height);
    cairo_line_to(cairo, t1, allocation.height - l1);
    cairo_line_to(cairo, t2, allocation.height - l2);
    cairo_line_to(cairo, t3, allocation.height - l3);
    cairo_line_to(cairo, allocation.width - t4, allocation.height - l3);
    cairo_line_to(cairo, allocation.width, allocation.height);
    cairo_stroke(cairo);

    /* draw dashed lines */

    cairo_set_line_width(cairo, 1);
    cairo_set_dash(cairo, dashes, 2, 0);
    cairo_set_line_cap(cairo, CAIRO_LINE_CAP_BUTT);
    cairo_set_line_join(cairo, CAIRO_LINE_JOIN_MITER);

    cairo_move_to(cairo, t1, allocation.height - 0);
    cairo_line_to(cairo, t1, allocation.height - l1);
    cairo_stroke(cairo);

    cairo_move_to(cairo, t2, allocation.height - 0);
    cairo_line_to(cairo, t2, allocation.height - l2);
    cairo_stroke(cairo);

    cairo_move_to(cairo, t3, allocation.height - 0);
    cairo_line_to(cairo, t3, allocation.height - l3);
    cairo_stroke(cairo);

    cairo_move_to(cairo, allocation.width - t4, allocation.height - 0);
    cairo_line_to(cairo, allocation.width - t4, allocation.height - l3);
    cairo_stroke(cairo);

    /* destroy graphics context */

    gdk_window_end_draw_frame(window, drawing_context);

    cairo_region_destroy(cairo_region);
}
