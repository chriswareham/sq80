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

#ifndef FILTER_H
#define FILTER_H

typedef struct {
    GtkWindow *dialog;
    GtkScale *frequency;
    GtkScale *resonance;
    GtkScale *keyboard_tracking;
    GtkComboBox *mod1_src;
    GtkScale *mod1_depth;
    GtkComboBox *mod2_src;
    GtkScale *mod2_depth;
} FilterDialog;

FilterDialog *new_filter_dialog(GtkWindow *);
void show_filter_dialog(FilterDialog *);
void set_filter_parameters(FilterDialog *, Patch *);
void clear_filter_parameters(FilterDialog *);

#endif /* !FILTER_H */
