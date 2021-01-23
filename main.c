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

#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>

#include "main.h"
#include "midi.h"
#include "dialog.h"
#include "device.h"
#include "oscillators.h"
#include "lfos.h"
#include "filter.h"
#include "envelopes.h"
#include "amplifier.h"
#include "modes.h"
#include "xmlparser.h"

Patch *current_patch = NULL;

enum { NAME_COL, TYPE_COL, DATA_COL, NCOLS };

typedef struct {
    GtkWidget *window;
    GtkWidget *oscillators_menu_item;
    GtkWidget *lfos_menu_item;
    GtkWidget *filter_menu_item;
    GtkWidget *envelopes_menu_item;
    GtkWidget *amplifier_menu_item;
    GtkWidget *modes_menu_item;
    GtkWidget *tree_view;
    GtkWidget *statusbar;
    guint statusbar_context_id;
    OscillatorsDialog *oscillators_dialog;
    LfosDialog *lfos_dialog;
    FilterDialog *filter_dialog;
    EnvelopesDialog *envelopes_dialog;
    AmplifierDialog *amplifier_dialog;
    ModesDialog *modes_dialog;
} MainWidgets;

static GtkWidget *create_file_menu(MainWidgets *);
static GtkWidget *create_edit_menu(MainWidgets *);
static GtkWidget *create_tree_view(MainWidgets *);
static void tree_view_callback(GtkTreeSelection *, gpointer);
static void show_oscillators_dialog_callback(GtkWidget *, gpointer);
static void show_lfos_dialog_callback(GtkWidget *, gpointer);
static void show_filter_dialog_callback(GtkWidget *, gpointer);
static void show_envelopes_dialog_callback(GtkWidget *, gpointer);
static void show_amplifier_dialog_callback(GtkWidget *, gpointer);
static void show_modes_dialog_callback(GtkWidget *, gpointer);
static void show_callback(GtkWidget *, gpointer);
static void new_callback(GtkWidget *, gpointer);
static void open_callback(GtkWidget *, gpointer);
static void save_callback(GtkWidget *, gpointer);
static void close_callback(GtkWidget *, gpointer);
static void quit_callback(GtkWidget *, gpointer);
static void destroy_callback(GtkWidget *, gpointer);
static void insert_patch(GtkWidget *, Patch *);
static void update_statusbar(MainWidgets *, const gchar *, ...);

int
main(int argc, char *argv[])
{
    MainWidgets widgets;
    GtkWidget *dialog, *vbox, *menu_bar, *menu_item, *scrolled_window;

    midi_initialise();

    gtk_init(&argc, &argv);

    if (midi_get_device_count() < 1) {
        dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "No MIDI devices found");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

    widgets.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(widgets.window), "SQ-80");
    gtk_window_set_default_size(GTK_WINDOW(widgets.window), 400, 400);
    gtk_container_set_border_width(GTK_CONTAINER(widgets.window), 0);
    g_signal_connect(G_OBJECT(widgets.window), "show", G_CALLBACK(show_callback), &widgets);
    g_signal_connect(G_OBJECT(widgets.window), "destroy", G_CALLBACK(destroy_callback), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(widgets.window), vbox);

    menu_bar = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, TRUE, 0);

    menu_item = gtk_menu_item_new_with_mnemonic("_File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), create_file_menu(&widgets));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item);

    menu_item = gtk_menu_item_new_with_mnemonic("_Edit");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), create_edit_menu(&widgets));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window), GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    widgets.tree_view = create_tree_view(&widgets);
    gtk_container_add(GTK_CONTAINER(scrolled_window), widgets.tree_view);

    widgets.statusbar = gtk_statusbar_new();
    widgets.statusbar_context_id = gtk_statusbar_get_context_id( GTK_STATUSBAR(widgets.statusbar), "MIDI device");
    gtk_box_pack_start(GTK_BOX(vbox), widgets.statusbar, FALSE, TRUE, 0);

    widgets.oscillators_dialog = new_oscillators_dialog(GTK_WINDOW(widgets.window));
    widgets.lfos_dialog = new_lfos_dialog(GTK_WINDOW(widgets.window));
    widgets.filter_dialog = new_filter_dialog(GTK_WINDOW(widgets.window));
    widgets.envelopes_dialog = new_envelopes_dialog(GTK_WINDOW(widgets.window));
    widgets.amplifier_dialog = new_amplifier_dialog(GTK_WINDOW(widgets.window));
    widgets.modes_dialog = new_modes_dialog(GTK_WINDOW(widgets.window));

    gtk_widget_show_all(widgets.window);

    gtk_main();

    return 0;
}

static GtkWidget *
create_file_menu(MainWidgets *widgets)
{
    GtkWidget *menu, *menu_item;

    menu = gtk_menu_new();

    menu_item = gtk_menu_item_new_with_mnemonic("_New");
    g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(new_callback), widgets);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_menu_item_new_with_mnemonic("_Open");
    g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(open_callback), widgets);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_menu_item_new_with_mnemonic("_Save");
    g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(save_callback), widgets);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_menu_item_new_with_mnemonic("_Close");
    g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(close_callback), widgets);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_menu_item_new_with_mnemonic("_Quit");
    g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(quit_callback), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    return menu;
}

static GtkWidget *
create_edit_menu(MainWidgets *widgets)
{
    GtkWidget *menu, *menu_item;

    menu = gtk_menu_new();

    menu_item = gtk_menu_item_new_with_mnemonic("_Device...");
    if (midi_get_device_count() > 0) {
        g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(device_dialog), widgets->window);
    } else {
        gtk_widget_set_sensitive(GTK_WIDGET(menu_item), FALSE);
    }
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    widgets->oscillators_menu_item = gtk_menu_item_new_with_mnemonic("_Oscillators...");
    g_signal_connect(G_OBJECT(widgets->oscillators_menu_item), "activate", G_CALLBACK(show_oscillators_dialog_callback), widgets);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->oscillators_menu_item), FALSE);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), widgets->oscillators_menu_item);

    widgets->lfos_menu_item = gtk_menu_item_new_with_mnemonic("_LFOs...");
    g_signal_connect(G_OBJECT(widgets->lfos_menu_item), "activate", G_CALLBACK(show_lfos_dialog_callback), widgets);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->lfos_menu_item), FALSE);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), widgets->lfos_menu_item);

    widgets->filter_menu_item = gtk_menu_item_new_with_mnemonic("_Filter...");
    g_signal_connect(G_OBJECT(widgets->filter_menu_item), "activate", G_CALLBACK(show_filter_dialog_callback), widgets);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->filter_menu_item), FALSE);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), widgets->filter_menu_item);

    widgets->envelopes_menu_item = gtk_menu_item_new_with_mnemonic("_Envelopes...");
    g_signal_connect(G_OBJECT(widgets->envelopes_menu_item), "activate", G_CALLBACK(show_envelopes_dialog_callback), widgets);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->envelopes_menu_item), FALSE);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), widgets->envelopes_menu_item);

    widgets->amplifier_menu_item = gtk_menu_item_new_with_mnemonic("_Amplifier...");
    g_signal_connect(G_OBJECT(widgets->amplifier_menu_item), "activate", G_CALLBACK(show_amplifier_dialog_callback), widgets);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->amplifier_menu_item), FALSE);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), widgets->amplifier_menu_item);

    widgets->modes_menu_item = gtk_menu_item_new_with_mnemonic("_Modes...");
    g_signal_connect(G_OBJECT(widgets->modes_menu_item), "activate", G_CALLBACK(show_modes_dialog_callback), widgets);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->modes_menu_item), FALSE);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), widgets->modes_menu_item);

    return menu;
}

static GtkWidget *
create_tree_view(MainWidgets *widgets)
{
    GtkWidget *tree_view;
    GtkListStore *store;
    GtkTreeSelection *selection;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new(NCOLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER);

    tree_view = gtk_tree_view_new();
    gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(store));

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
    g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(tree_view_callback), widgets);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    return tree_view;
}

static void
tree_view_callback(GtkTreeSelection *selection, gpointer data)
{
    MainWidgets *widgets = data;
    GtkTreeIter iter;
    GtkTreeModel *model;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, DATA_COL, &current_patch, -1);

        set_oscillators_parameters(widgets->oscillators_dialog, current_patch);
        set_lfos_parameters(widgets->lfos_dialog, current_patch);
        set_filter_parameters(widgets->filter_dialog, current_patch);
        set_envelopes_parameters(widgets->envelopes_dialog, current_patch);
        set_amplifier_parameters(widgets->amplifier_dialog, current_patch);
        set_modes_parameters(widgets->modes_dialog, current_patch);

        gtk_widget_set_sensitive(GTK_WIDGET(widgets->oscillators_menu_item), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->lfos_menu_item), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->filter_menu_item), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->envelopes_menu_item), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->amplifier_menu_item), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->modes_menu_item), TRUE);
    } else {
        current_patch = NULL;

        clear_oscillators_parameters(widgets->oscillators_dialog);
        clear_lfos_parameters(widgets->lfos_dialog);
        clear_filter_parameters(widgets->filter_dialog);
        clear_envelopes_parameters(widgets->envelopes_dialog);
        clear_amplifier_parameters(widgets->amplifier_dialog);
        clear_modes_parameters(widgets->modes_dialog);

        gtk_widget_set_sensitive(GTK_WIDGET(widgets->oscillators_menu_item), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->lfos_menu_item), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->filter_menu_item), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->envelopes_menu_item), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->amplifier_menu_item), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->modes_menu_item), FALSE);
    }
}

static void
show_oscillators_dialog_callback(GtkWidget *widget, gpointer data)
{
    MainWidgets *widgets = data;
    show_oscillators_dialog(widgets->oscillators_dialog);
}

static void
show_lfos_dialog_callback(GtkWidget *widget, gpointer data)
{
    MainWidgets *widgets = data;
    show_lfos_dialog(widgets->lfos_dialog);
}

static void
show_filter_dialog_callback(GtkWidget *widget, gpointer data)
{
    MainWidgets *widgets = data;
    show_filter_dialog(widgets->filter_dialog);
}

static void
show_envelopes_dialog_callback(GtkWidget *widget, gpointer data)
{
    MainWidgets *widgets = data;
    show_envelopes_dialog(widgets->envelopes_dialog);
}

static void
show_amplifier_dialog_callback(GtkWidget *widget, gpointer data)
{
    MainWidgets *widgets = data;
    show_amplifier_dialog(widgets->amplifier_dialog);
}


static void
show_modes_dialog_callback(GtkWidget *widget, gpointer data)
{
    MainWidgets *widgets = data;
    show_modes_dialog(widgets->modes_dialog);
}

static void
show_callback(GtkWidget *widget, gpointer data)
{
    MainWidgets *widgets;
    GtkTreeViewColumn *name_column, *type_column;
    gint width;
    MIDIDevice **midi_devices;

    widgets = data;

    name_column = gtk_tree_view_get_column(GTK_TREE_VIEW(widgets->tree_view), 0);
    type_column = gtk_tree_view_get_column(GTK_TREE_VIEW(widgets->tree_view), 1);

    width = gtk_tree_view_column_get_width(GTK_TREE_VIEW_COLUMN(name_column));
    width += gtk_tree_view_column_get_width(GTK_TREE_VIEW_COLUMN(type_column));

    gtk_tree_view_column_set_min_width(GTK_TREE_VIEW_COLUMN(name_column), width - (width / 3));
    gtk_tree_view_column_set_min_width(GTK_TREE_VIEW_COLUMN(type_column), width / 3);

    midi_devices = midi_get_devices();

    if (midi_get_device_count() && midi_open(midi_devices[0])) {
        update_statusbar(widgets, "MIDI device %s", midi_devices[0]->name);
    } else {
        update_statusbar(widgets, "MIDI device : none");
    }
}

static void
new_callback(GtkWidget *widget, gpointer data)
{
    MainWidgets *widgets;
    GtkWidget *dialog, *label, *name, *type;
    GtkGrid *grid;
    Patch *patch;

    widgets = data;

    dialog = gtk_dialog_new_with_buttons("New Patch",
        GTK_WINDOW(widgets->window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL);

    grid = create_grid(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))));

    label = gtk_label_new("Name:");
    name = gtk_entry_new();
    create_grid_row(GTK_GRID(grid), 0, GTK_LABEL(label), name);

    label = gtk_label_new("Type:");
    type = gtk_entry_new();
    create_grid_row(GTK_GRID(grid), 1, GTK_LABEL(label), type);

    gtk_widget_show_all(GTK_WIDGET(grid));

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK && strlen(gtk_entry_get_text(GTK_ENTRY(name)))) {
        patch = g_malloc0(sizeof(Patch));
        patch->name = g_strdup(gtk_entry_get_text(GTK_ENTRY(name)));
        patch->type = g_strdup(gtk_entry_get_text(GTK_ENTRY(type)));

        patch->parameters[PARAMETER_OSC1_MOD1_SRC] = 15;
        patch->parameters[PARAMETER_OSC1_MOD2_SRC] = 15;

        patch->parameters[PARAMETER_OSC2_MOD1_SRC] = 15;
        patch->parameters[PARAMETER_OSC2_MOD2_SRC] = 15;

        patch->parameters[PARAMETER_OSC3_MOD1_SRC] = 15;
        patch->parameters[PARAMETER_OSC3_MOD2_SRC] = 15;

        patch->parameters[PARAMETER_DCA1_OUTPUT] = 1;
        patch->parameters[PARAMETER_DCA1_MOD1_SRC] = 15;
        patch->parameters[PARAMETER_DCA1_MOD2_SRC] = 15;

        patch->parameters[PARAMETER_DCA2_OUTPUT] = 1;
        patch->parameters[PARAMETER_DCA2_MOD1_SRC] = 15;
        patch->parameters[PARAMETER_DCA2_MOD2_SRC] = 15;

        patch->parameters[PARAMETER_DCA3_OUTPUT] = 1;
        patch->parameters[PARAMETER_DCA3_MOD1_SRC] = 15;
        patch->parameters[PARAMETER_DCA3_MOD2_SRC] = 15;

        patch->parameters[PARAMETER_LFO1_MOD_SRC] = 15;
        patch->parameters[PARAMETER_LFO2_MOD_SRC] = 15;
        patch->parameters[PARAMETER_LFO3_MOD_SRC] = 15;

        patch->parameters[PARAMETER_FILTER_FREQUENCY] = 127;
        patch->parameters[PARAMETER_FILTER_MOD1_SRC] = 15;
        patch->parameters[PARAMETER_FILTER_MOD2_SRC] = 15;

        patch->parameters[PARAMETER_DCA4_ENV4_DEPTH] = 63;
        patch->parameters[PARAMETER_DCA4_PAN] = 8;
        patch->parameters[PARAMETER_DCA4_MOD_SRC] = 15;

        insert_patch(widgets->tree_view, patch);
    }

    gtk_widget_destroy(dialog);
}

static void
open_callback(GtkWidget *widget, gpointer data)
{
    MainWidgets *widgets;
    GtkWidget *dialog, *message_dialog;
    gchar *filename;
    GError *error = NULL;
    Patch *patch;

    widgets = data;

    dialog = gtk_file_chooser_dialog_new("Open Patch",
        GTK_WINDOW(widgets->window),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Open", GTK_RESPONSE_ACCEPT,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        patch = xmlparser_read(filename, &error);

        if (patch) {
            patch->filename = filename;
            insert_patch(widgets->tree_view, patch);
        } else {
            message_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_CLOSE,
                "Unable to load %s", filename);
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);

            g_print("Unable to load %s:\n%s\n", filename, error->message);

            g_error_free(error);

            g_free(filename);
        }
    }

    gtk_widget_destroy(dialog);
}

static void
save_callback(GtkWidget *widget, gpointer data)
{
    MainWidgets *widgets;
    GtkWidget *dialog, *message_dialog;
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    Patch *patch;

    widgets = data;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widgets->tree_view));

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, DATA_COL, &patch, -1);

        if (!patch->filename) {
            dialog = gtk_file_chooser_dialog_new("Save Patch",
                GTK_WINDOW(widgets->window),
                GTK_FILE_CHOOSER_ACTION_SAVE,
                "_Save", GTK_RESPONSE_ACCEPT,
                "_Cancel", GTK_RESPONSE_CANCEL,
                NULL);

            if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
                patch->filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            }

            gtk_widget_destroy(dialog);
        }

        if (patch->filename && !xmlparser_write(patch->filename, patch)) {
            message_dialog = gtk_message_dialog_new(GTK_WINDOW(widgets->window),
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_CLOSE,
                "Unable to save %s", patch->filename);
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);

            g_print("Unable to load %s:\n%s\n", patch->filename, strerror(errno));
        }
    }
}

static void
close_callback(GtkWidget *widget, gpointer data)
{
    MainWidgets *widgets;
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    Patch *patch;

    widgets = data;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widgets->tree_view));

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, DATA_COL, &patch, -1);

        gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

        g_free(patch->name);
        g_free(patch->type);
        if (patch->filename) {
            g_free(patch->filename);
        }
        g_free(patch);
    }
}

static void
quit_callback(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

static void
destroy_callback(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

static void
insert_patch(GtkWidget *tree_view, Patch *new_patch)
{
    GtkTreeModel *model;
    GtkTreeIter new_iter, iter;
    GtkTreeSelection *selection;
    gboolean valid;
    Patch *patch;

    model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));

    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(model), &iter);

    while (valid) {
        gtk_tree_model_get(GTK_TREE_MODEL(model), &iter, DATA_COL, &patch, -1);

        if (patch->filename && new_patch->filename && strcmp(patch->filename, new_patch->filename) == 0) {
            new_iter = iter;
            g_free(new_patch->name);
            g_free(new_patch->type);
            g_free(new_patch->filename);
            g_free(new_patch);
            break;
        }

        if (g_ascii_strcasecmp(patch->name, new_patch->name) > 0) {
            gtk_list_store_insert_before(GTK_LIST_STORE(model), &new_iter, &iter);
            gtk_list_store_set(GTK_LIST_STORE(model), &new_iter,
                NAME_COL, new_patch->name,
                TYPE_COL, new_patch->type,
                DATA_COL, new_patch,
                -1);
            break;
        }

        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(model), &iter);
    }

    if (!valid) {
        gtk_list_store_append(GTK_LIST_STORE(model), &new_iter);
        gtk_list_store_set(GTK_LIST_STORE(model), &new_iter,
            NAME_COL, new_patch->name,
            TYPE_COL, new_patch->type,
            DATA_COL, new_patch,
            -1);
    }

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    gtk_tree_selection_select_iter(GTK_TREE_SELECTION(selection), &new_iter);
}

void
update_statusbar(MainWidgets *widgets, const gchar *fmt, ...)
{
    va_list args;
    gchar *str;

    gtk_statusbar_pop(GTK_STATUSBAR(widgets->statusbar), widgets->statusbar_context_id);

    va_start(args, fmt);

    g_vasprintf(&str, fmt, args);

    gtk_statusbar_push(GTK_STATUSBAR(widgets->statusbar), widgets->statusbar_context_id, str);

    g_free(str);

    va_end(args);
}
