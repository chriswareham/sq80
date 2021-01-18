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

#ifndef OSCILLATORS_H
#define OSCILLATORS_H

typedef struct {
    /* OSC */
    GtkScale *octave;
    GtkScale *semitone;
    GtkScale *fine;
    GtkComboBox *wave;
    GtkComboBox *mod1_src;
    GtkScale *mod1_depth;
    GtkComboBox *mod2_src;
    GtkScale *mod2_depth;

    /* DCA */
    GtkScale *dca_level;
    GtkCheckButton *dca_output;
    GtkComboBox *dca_mod1_src;
    GtkScale *dca_mod1_depth;
    GtkComboBox *dca_mod2_src;
    GtkScale *dca_mod2_depth;
} Oscillator;

typedef struct {
    GtkWindow *dialog;
    Oscillator osc1;
    Oscillator osc2;
    Oscillator osc3;
} OscillatorsDialog;

OscillatorsDialog *new_oscillators_dialog(GtkWindow *);
void show_oscillators_dialog(OscillatorsDialog *);
void set_oscillators_parameters(OscillatorsDialog *, Patch *);
void clear_oscillators_parameters(OscillatorsDialog *);

#endif /* !OSCILLATORS_H */
