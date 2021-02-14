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

static gboolean delete_window_callback(GtkWidget *, GdkEvent *, gpointer);

/**
   \brief Creates a dialog window.

   \param parent - the parent window of the dialog.
   \param title - the title of the dialog.
   \param modal - whether the dialog is modal.
   \return the newly created dialog window.
 */
GtkWindow *
create_window(GtkWindow *parent, const gchar *title, gboolean modal)
{
    GtkWidget *window;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_container_set_border_width(GTK_CONTAINER(window), 6);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(delete_window_callback), window);

    if (modal) {
        gtk_window_set_modal(GTK_WINDOW(window), TRUE);
        gtk_window_set_transient_for(GTK_WINDOW(window), parent);
    }

    return GTK_WINDOW(window);
}

/**
   \brief Popups a window or brings it to the front if it's already popped up.

   \param window - the window to popup or bring to the front.
 */
void
show_window(GtkWindow *window)
{
    if (gtk_widget_get_visible(GTK_WIDGET(window))) {
        gtk_window_present(window);
    } else {
        gtk_widget_show_all(GTK_WIDGET(window));
    }
}

/**
   \brief Creates a grid.

   \param parent - the parent container of the grid.
   \return the newly created grid.
 */
GtkGrid *
create_grid(GtkContainer *parent)
{
    GtkWidget *grid;

    grid = gtk_grid_new();
    gtk_container_set_border_width(GTK_CONTAINER(grid), 6);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 6);
    if (GTK_IS_BOX(parent)) {
        gtk_box_pack_start(GTK_BOX(parent), grid, FALSE, FALSE, 0);
    } else {
        gtk_container_add(parent, grid);
    }

    return GTK_GRID(grid);
}

/**
   \brief Adds a new row containing a label and another widget to a grid.

   \param grid - the grid to add the row of widgets to.
   \param row - the row number (indexed from zero).
   \param label - the label to add.
   \param widget - the other widget to add.
 */
void
create_grid_row(GtkGrid *grid, gint row, GtkLabel *label, GtkWidget *widget)
{
    gtk_label_set_xalign(label, 0.5);
    gtk_label_set_yalign(label, 0.5);
    gtk_grid_attach(grid, GTK_WIDGET(label), 0, row, 1, 1);
    gtk_grid_attach(grid, widget, 1, row, 1, 1);
}

/**
   \brief Creates a horizontal scale widget to edit a patch parameter.

   \param minimum - the minumum value of the patch parameter.
   \param maximum - the maximum value of the patch parameter.
   \param parameter - the patch parameter.
   \return the newly created horizontal scale widget.
 */
GtkScale *
create_hscale(gint minimum, gint maximum, gint parameter)
{
    GtkWidget *hscale;

    hscale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, minimum, maximum, 1);
    gtk_widget_set_hexpand(hscale, TRUE);
    gtk_widget_set_halign(hscale, GTK_ALIGN_FILL);
    g_signal_connect(G_OBJECT(hscale), "button-release-event", G_CALLBACK(hscale_callback), GINT_TO_POINTER(parameter));
    g_signal_connect(G_OBJECT(hscale), "key-release-event", G_CALLBACK(hscale_callback), GINT_TO_POINTER(parameter));

    return GTK_SCALE(hscale);
}

/**
   \brief Creates a horizontal scale widget to edit a patch parameter.

   \param minimum - the minumum value of the patch parameter.
   \param maximum - the maximum value of the patch parameter.
   \param params - the patch parameters.
   \return the newly created horizontal scale widget.
 */
GtkScale *
create_hscale_with_params(gint minimum, gint maximum, ScaleParams *params)
{
    GtkWidget *hscale;

    hscale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, minimum, maximum, 1);
    gtk_widget_set_hexpand(hscale, TRUE);
    gtk_widget_set_halign(hscale, GTK_ALIGN_FILL);
    g_signal_connect(G_OBJECT(hscale), "button-release-event", G_CALLBACK(hscale_callback_with_params), params);
    g_signal_connect(G_OBJECT(hscale), "key-release-event", G_CALLBACK(hscale_callback_with_params), params);

    return GTK_SCALE(hscale);
}

/**
   \brief Creates a combo box widget to edit a patch parameter.

   \param entries - the array of strings to populate the combo box with.
   \param entry_count - the length of the array of strings.
   \param parameter - the patch parameter.
   \return the newly created combo box widget.
 */
GtkComboBox *
create_combo_box(gchar *entries[], gint entry_count, gint parameter)
{
    gint i;
    GtkListStore *store;
    GtkTreeIter iter;
    GtkWidget *combo_box;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new(1, G_TYPE_STRING);

    for (i = 0; i < entry_count; ++i) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, entries[i], -1);
    }

    combo_box = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
    g_signal_connect(G_OBJECT(combo_box), "changed", G_CALLBACK(combo_box_callback), GINT_TO_POINTER(parameter));

    renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_box), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_box), renderer, "text", 0, NULL);

    return GTK_COMBO_BOX(combo_box);
}

/**
   \brief Creates a combo box widget to edit a patch parameter.

   \param entries - the array of entries to populate the combo box with.
   \param entry_count - the length of the array of entries.
   \return the newly created combo box widget.
 */
GtkComboBox *
create_combo_box_with_entries(ComboBoxEntry *entries, gint entry_count)
{
    gint i;
    GtkListStore *store;
    GtkTreeIter iter;
    GtkWidget *combo_box;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new(1, G_TYPE_STRING);

    for (i = 0; i < entry_count; ++i) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, entries[i].label, -1);
    }

    combo_box = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
    g_signal_connect(G_OBJECT(combo_box), "changed", G_CALLBACK(combo_box_with_entries_callback), entries);

    renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_box), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_box), renderer, "text", 0, NULL);

    return GTK_COMBO_BOX(combo_box);
}

/**
   \brief Creates a check button widget to edit a patch parameter.

   \param parameter - the patch parameter.
   \return the newly created check button widget.
 */
GtkCheckButton *
create_check_button(gint parameter)
{
    GtkWidget *check_button;

    check_button = gtk_check_button_new();
    g_signal_connect(G_OBJECT(check_button), "toggled", G_CALLBACK(check_button_callback), GINT_TO_POINTER(parameter));

    return GTK_CHECK_BUTTON(check_button);
}

/**
   \brief Callback for a horizontal scale widget to edit a patch parameter.

   \param widget - the horizontal scale widget.
   \param event - the event that triggered the callback.
   \param data - the data associated with the callback.
   \return whether to stop other handlers from being invoked for the event.
 */
gboolean
hscale_callback(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gint parameter, value;
    MIDIMessage msg[3];

    parameter = GPOINTER_TO_INT(data);

    value = gtk_range_get_value(GTK_RANGE(widget));

    fprintf(stderr, "Parameter %d value %d\n", parameter, value);

    if (current_patch) {
        current_patch->parameters[parameter] = (guchar) value;
    }

    msg[0].status = 0xb0;
    msg[0].data1 = 0x62;
    msg[0].data2 = (guchar) parameter;

    msg[1].status = 0xb0;
    msg[1].data1 = 0x63;
    msg[1].data2 = 0x00;

    msg[2].status = 0xb0;
    msg[2].data1 = 0x06;
    msg[2].data2 = (guchar) value;

    midi_write(msg, 3);

    return FALSE;
}

/**
   \brief Callback for a horizontal scale widget to edit a patch parameter.

   \param widget - the horizontal scale widget.
   \param event - the event that triggered the callback.
   \param data - the data associated with the callback.
   \return whether to stop other handlers from being invoked for the event.
 */
gboolean
hscale_callback_with_params(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gint parameter, value;
    MIDIMessage msg[3];

    ScaleParams *params = (ScaleParams *) data;

    parameter = params->parameter;

    value = gtk_range_get_value(GTK_RANGE(widget));

    fprintf(stderr, "Parameter %d value %d\n", parameter, value);

    if (current_patch) {
        current_patch->parameters[parameter] = (guchar) value;
    }

    value += params->offset;

    if (params->multiplier > 0) {
        value *= params->multiplier;
    }

    msg[0].status = 0xb0;
    msg[0].data1 = 0x62;
    msg[0].data2 = (guchar) parameter;

    msg[1].status = 0xb0;
    msg[1].data1 = 0x63;
    msg[1].data2 = 0x00;

    msg[2].status = 0xb0;
    msg[2].data1 = 0x06;
    msg[2].data2 = (guchar) value;

    midi_write(msg, 3);

    return FALSE;
}

/**
   \brief Callback for a combo box widget to edit a patch parameter.

   \param widget - the combo box widget.
   \param data - the data associated with the callback.
 */
void
combo_box_callback(GtkWidget *widget, gpointer data)
{
    gint parameter, value;
    MIDIMessage msg[3];

    parameter = GPOINTER_TO_INT(data);

    value = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));

    fprintf(stderr, "Parameter %d value %d\n", parameter, value);

    if (current_patch) {
        current_patch->parameters[parameter] = (guchar) value;
    }

    msg[0].status = 0xb0;
    msg[0].data1 = 0x62;
    msg[0].data2 = (guchar) parameter;

    msg[1].status = 0xb0;
    msg[1].data1 = 0x63;
    msg[1].data2 = 0x00;

    msg[2].status = 0xb0;
    msg[2].data1 = 0x06;
    msg[2].data2 = (guchar) value;

    midi_write(msg, 3);
}

/**
   \brief Callback for a combo box widget to edit a patch parameter.

   \param widget - the combo box widget.
   \param data - the data associated with the callback.
 */
void
combo_box_with_entries_callback(GtkWidget *widget, gpointer data)
{
    gint parameter, value, i;
    MIDIMessage msg[3];

    ComboBoxEntry *entry = (ComboBoxEntry *) data;

    i = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));

    parameter = entry[i].parameter;

    value = entry[i].value;

    fprintf(stderr, "Parameter %d value %d\n", parameter, value);

    if (current_patch) {
        current_patch->parameters[parameter] = (guchar) value;
    }

    msg[0].status = 0xb0;
    msg[0].data1 = 0x62;
    msg[0].data2 = (guchar) parameter;

    msg[1].status = 0xb0;
    msg[1].data1 = 0x63;
    msg[1].data2 = 0x00;

    msg[2].status = 0xb0;
    msg[2].data1 = 0x06;
    msg[2].data2 = (guchar) value;

    midi_write(msg, 3);
}

/**
   \brief Callback for a check button widget to edit a patch parameter.

   \param widget - the check button widget.
   \param data - the data associated with the callback.
 */
void
check_button_callback(GtkWidget *widget, gpointer data)
{
    gint parameter;
    gboolean value;
    MIDIMessage msg[3];

    parameter = GPOINTER_TO_INT(data);

    value = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

    fprintf(stderr, "Parameter %d value %d\n", parameter, value ? 1 : 0);

    if (current_patch) {
        current_patch->parameters[parameter] = value ? 1 : 0;
    }

    msg[0].status = 0xb0;
    msg[0].data1 = 0x62;
    msg[0].data2 = (guchar) parameter;

    msg[1].status = 0xb0;
    msg[1].data1 = 0x63;
    msg[1].data2 = 0x00;

    msg[2].status = 0xb0;
    msg[2].data1 = 0x06;
    msg[2].data2 = value ? 0x40 : 0x00;

    midi_write(msg, 3);
}

/**
   \brief Callback to popdown a window.

   \param widget - the widget that activated the callback.
   \param data - callback data (the window to popdown).
 */
void
close_window_callback(GtkWidget *widget, gpointer data)
{
    gtk_widget_hide(GTK_WIDGET(data));
}

static gboolean
delete_window_callback(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_widget_hide(GTK_WIDGET(data));

    return TRUE;
}
