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

#include "midi.h"
#include "main.h"
#include "dialog.h"
#include "device.h"

static gchar *program_banks[] = {
    "Internal",
    "Cartridge A",
    "Cartridge B"
};

typedef struct {
    GtkWindow *dialog;
    GtkWidget *device;
    GtkWidget *channel;
    GtkWidget *program_bank;
    GtkWidget *program_number;
    GtkWidget *note;
    GtkWidget *velocity;
    GtkWidget *on_button;
    GtkWidget *off_button;
    GtkWidget *close_button;
} DeviceWidgets;

static GtkWidget *device_combo_box(void);
static GtkWidget *program_bank_combo_box(void);
static void device_callback(GtkWidget *, gpointer);
static void note_on_callback(GtkWidget *, gpointer);
static void note_off_callback(GtkWidget *, gpointer);

void
device_dialog(GtkWindow *window, Statusbar *statusbar)
{
    static DeviceWidgets *widgets = NULL;
    GtkGrid *grid;
    GtkWidget *button_box, *label;

    if (widgets == NULL) {
        widgets = g_new(DeviceWidgets, 1);

        widgets->dialog = create_window(window, "Device", TRUE);

        grid = create_grid(GTK_CONTAINER(widgets->dialog));

        label = gtk_label_new("Device:");
        widgets->device = device_combo_box();
        g_signal_connect(G_OBJECT(widgets->device), "changed", G_CALLBACK(device_callback), statusbar);
        create_grid_row(grid, 0, GTK_LABEL(label), widgets->device);

        label = gtk_label_new("Channel:");
        widgets->channel = gtk_spin_button_new_with_range(1.0, 16.0, 1.0);
        create_grid_row(grid, 1, GTK_LABEL(label), widgets->channel);

        label = gtk_label_new("Program bank:");
        widgets->program_bank = program_bank_combo_box();
        create_grid_row(grid, 2, GTK_LABEL(label), widgets->program_bank);

        label = gtk_label_new("Program number:");
        widgets->program_number = gtk_spin_button_new_with_range(1.0, 40.0, 1.0);
        create_grid_row(grid, 3, GTK_LABEL(label), widgets->program_number);

        label = gtk_label_new("Note:");
        widgets->note = gtk_spin_button_new_with_range(0.0, 127.0, 1.0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(widgets->note), 60.0);
        create_grid_row(grid, 4, GTK_LABEL(label), widgets->note);

        label = gtk_label_new("Velocity:");
        widgets->velocity = gtk_spin_button_new_with_range(0.0, 127.0, 1.0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(widgets->velocity), 64.0);
        create_grid_row(grid, 5, GTK_LABEL(label), widgets->velocity);

        button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_box_set_spacing(GTK_BOX(button_box), 6);
        gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
        gtk_grid_attach(grid, button_box, 0, 6, 2, 1);

        widgets->on_button = gtk_button_new_with_label("Note On");
        g_signal_connect(G_OBJECT(widgets->on_button), "clicked", G_CALLBACK(note_on_callback), widgets);
        gtk_container_add(GTK_CONTAINER(button_box), widgets->on_button);

        widgets->off_button = gtk_button_new_with_label("Note Off");
        g_signal_connect(G_OBJECT(widgets->off_button), "clicked", G_CALLBACK(note_off_callback), widgets);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->off_button), FALSE);
        gtk_container_add(GTK_CONTAINER(button_box), widgets->off_button);

        widgets->close_button = gtk_button_new_with_label("Close");
        g_signal_connect(G_OBJECT(widgets->close_button), "clicked", G_CALLBACK(close_window_callback), widgets->dialog);
        gtk_container_add(GTK_CONTAINER(button_box), widgets->close_button);
    }

    gtk_widget_show_all(GTK_WIDGET(widgets->dialog));
}

static GtkWidget *
device_combo_box(void)
{
    MIDIDevice **midi_devices;
    gint i;
    GtkListStore *store;
    GtkTreeIter iter;
    GtkWidget *combo_box;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new(1, G_TYPE_STRING);

    if ((midi_devices = midi_get_devices())) {
        for (i = 0; midi_devices[i]; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, midi_devices[i]->name, -1);
        }
    }

    combo_box = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));

    renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_box), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_box), renderer, "text", 0, NULL);

    if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter)) {
        gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo_box), &iter);
    }

    return combo_box;
}

static GtkWidget *
program_bank_combo_box(void)
{
    gint i;
    GtkListStore *store;
    GtkTreeIter iter;
    GtkWidget *combo_box;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new(1, G_TYPE_STRING);

    for (i = 0; i < G_N_ELEMENTS(program_banks); i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, program_banks[i], -1);
    }

    combo_box = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));

    renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_box), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_box), renderer, "text", 0, NULL);

    if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter)) {
        gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo_box), &iter);
    }

    return combo_box;
}

static void
device_callback(GtkWidget *widget, gpointer data)
{
    gint i;
    MIDIDevice **midi_devices;
    Statusbar *statusbar;

    statusbar = data;

    i = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));

    if (i != -1) {
        midi_close();
        midi_devices = midi_get_devices();
        if (midi_open(midi_devices[i])) {
            update_statusbar(statusbar, midi_devices[i]->name);
        } else {
            update_statusbar(statusbar, "None");
        }
    }
}

static void
note_on_callback(GtkWidget *widget, gpointer data)
{
    DeviceWidgets *widgets = data;
    guchar channel, program, note, velocity;

    channel = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->channel)) - 1;
    program = gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->program_bank)) * 40;
    program += gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->program_number)) - 1;
    note = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->note));
    velocity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->velocity));

    midi_program_change(channel, program);

    midi_note_on(channel, note, velocity);

    gtk_widget_set_sensitive(GTK_WIDGET(widgets->on_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->off_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->close_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->device), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->channel), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->program_bank), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->program_number), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->note), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->velocity), FALSE);
}

static void
note_off_callback(GtkWidget *widget, gpointer data)
{
    DeviceWidgets *widgets = data;
    guchar channel, note, velocity;

    channel = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->channel)) - 1;
    note = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->note));
    velocity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->velocity));

    midi_note_off(channel, note, velocity);

    gtk_widget_set_sensitive(GTK_WIDGET(widgets->on_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->off_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->close_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->device), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->channel), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->program_bank), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->program_number), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->note), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->velocity), TRUE);
}
