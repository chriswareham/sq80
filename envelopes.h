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

#ifndef ENVELOPES_H
#define ENVELOPES_H
typedef struct {
    GtkWindow *dialog;
    GtkWidget *envelope;
    GtkScale *level1;
    GtkScale *level2;
    GtkScale *level3;
    GtkScale *velocity_level;
    GtkScale *velocity_attack;
    GtkScale *time1;
    GtkScale *time2;
    GtkScale *time3;
    GtkScale *time4;
    GtkScale *keyboard_decay_scaling;
} Envelope;

typedef struct {
    GtkWindow *dialog;
    Envelope env1;
    Envelope env2;
    Envelope env3;
    Envelope env4;
} EnvelopesDialog;

EnvelopesDialog *new_envelopes_dialog(GtkWindow *);
void show_envelopes_dialog(EnvelopesDialog *);
void set_envelopes_parameters(EnvelopesDialog *, Patch *);
void clear_envelopes_parameters(EnvelopesDialog *);

#endif /* !ENVELOPES_H */
