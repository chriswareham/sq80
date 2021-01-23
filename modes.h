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

#ifndef MODES_H
#define MODES_H

typedef struct {
    GtkWindow *dialog;
    GtkCheckButton *amplitude_modulation;
    GtkScale *glide;
    GtkCheckButton *mono;
    GtkCheckButton *sync;
    GtkCheckButton *voice_restart;
    GtkCheckButton *envelope_restart;
    GtkCheckButton *oscillator_restart;
    GtkCheckButton *envelope_full_cycle;
} ModesDialog;

ModesDialog *new_modes_dialog(GtkWindow *);
void show_modes_dialog(ModesDialog *);
void set_modes_parameters(ModesDialog *, Patch *);
void clear_modes_parameters(ModesDialog *);

#endif /* !MODES_H */
