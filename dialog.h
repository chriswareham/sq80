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

#ifndef DIALOG_H
#define DIALOG_H

typedef struct {
    guchar parameter;
    gint multiplier;
    gint offset;
} ScaleParams;

typedef struct {
    gchar *label;
    guchar parameter;
    guchar value;
} ComboBoxEntry;

GtkWindow *create_window(GtkWindow *, const gchar *, gboolean);
void show_window(GtkWindow *);
GtkGrid *create_grid(GtkContainer *);
void create_grid_row(GtkGrid *, gint, GtkLabel *, GtkWidget *);

GtkScale *create_hscale(gint, gint, gint);
GtkScale *create_hscale_with_params(gint, gint, ScaleParams *);
GtkComboBox *create_combo_box(gchar **, gint, gint);
GtkComboBox *create_combo_box_with_entries(ComboBoxEntry *, gint);
GtkCheckButton *create_check_button(gint);

gboolean hscale_callback(GtkWidget *, GdkEvent *, gpointer);
gboolean hscale_callback_with_params(GtkWidget *, GdkEvent *, gpointer);
void combo_box_callback(GtkWidget *, gpointer);
void combo_box_with_entries_callback(GtkWidget *, gpointer);
void check_button_callback(GtkWidget *, gpointer);
void close_window_callback(GtkWidget *, gpointer);

#endif /* !DIALOG_H */
