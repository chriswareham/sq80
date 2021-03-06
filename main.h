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

#ifndef MAIN_H
#define MAIN_H

typedef enum {
    /* Envelope 1 */

    PARAMETER_ENV1_LEVEL1,
    PARAMETER_ENV1_LEVEL2,
    PARAMETER_ENV1_LEVEL3,
    PARAMETER_ENV1_VELOCITY_LEVEL,
    PARAMETER_ENV1_VELOCITY_ATTACK,
    PARAMETER_ENV1_TIME1,
    PARAMETER_ENV1_TIME2,
    PARAMETER_ENV1_TIME3,
    PARAMETER_ENV1_TIME4,
    PARAMETER_ENV1_KEYBOARD_DECAY_SCALING,

    /* Envelope 2 */

    PARAMETER_ENV2_LEVEL1,
    PARAMETER_ENV2_LEVEL2,
    PARAMETER_ENV2_LEVEL3,
    PARAMETER_ENV2_VELOCITY_LEVEL,
    PARAMETER_ENV2_VELOCITY_ATTACK,
    PARAMETER_ENV2_TIME1,
    PARAMETER_ENV2_TIME2,
    PARAMETER_ENV2_TIME3,
    PARAMETER_ENV2_TIME4,
    PARAMETER_ENV2_KEYBOARD_DECAY_SCALING,

    /* Envelope 3 */

    PARAMETER_ENV3_LEVEL1,
    PARAMETER_ENV3_LEVEL2,
    PARAMETER_ENV3_LEVEL3,
    PARAMETER_ENV3_VELOCITY_LEVEL,
    PARAMETER_ENV3_VELOCITY_ATTACK,
    PARAMETER_ENV3_TIME1,
    PARAMETER_ENV3_TIME2,
    PARAMETER_ENV3_TIME3,
    PARAMETER_ENV3_TIME4,
    PARAMETER_ENV3_KEYBOARD_DECAY_SCALING,

    /* Envelope 4 */

    PARAMETER_ENV4_LEVEL1,
    PARAMETER_ENV4_LEVEL2,
    PARAMETER_ENV4_LEVEL3,
    PARAMETER_ENV4_VELOCITY_LEVEL,
    PARAMETER_ENV4_VELOCITY_ATTACK,
    PARAMETER_ENV4_TIME1,
    PARAMETER_ENV4_TIME2,
    PARAMETER_ENV4_TIME3,
    PARAMETER_ENV4_TIME4,
    PARAMETER_ENV4_KEYBOARD_DECAY_SCALING,

    /* LFO 1 */

    PARAMETER_LFO1_FREQUENCY,
    PARAMETER_LFO1_RESET,
    PARAMETER_LFO1_HUMAN,
    PARAMETER_LFO1_WAVE,
    PARAMETER_LFO1_INITIAL_LEVEL,
    PARAMETER_LFO1_DELAY,
    PARAMETER_LFO1_FINAL_LEVEL,
    PARAMETER_LFO1_MOD_SRC,

    /* LFO 2 */

    PARAMETER_LFO2_FREQUENCY,
    PARAMETER_LFO2_RESET,
    PARAMETER_LFO2_HUMAN,
    PARAMETER_LFO2_WAVE,
    PARAMETER_LFO2_INITIAL_LEVEL,
    PARAMETER_LFO2_DELAY,
    PARAMETER_LFO2_FINAL_LEVEL,
    PARAMETER_LFO2_MOD_SRC,

    /* LFO 3 */

    PARAMETER_LFO3_FREQUENCY,
    PARAMETER_LFO3_RESET,
    PARAMETER_LFO3_HUMAN,
    PARAMETER_LFO3_WAVE,
    PARAMETER_LFO3_INITIAL_LEVEL,
    PARAMETER_LFO3_DELAY,
    PARAMETER_LFO3_FINAL_LEVEL,
    PARAMETER_LFO3_MOD_SRC,

    /* Oscillator 1 */

    PARAMETER_OSC1_OCTAVE,
    PARAMETER_OSC1_SEMITONE,
    PARAMETER_OSC1_FINE,
    PARAMETER_OSC1_WAVE,
    PARAMETER_OSC1_MOD1_SRC,
    PARAMETER_OSC1_MOD1_DEPTH,
    PARAMETER_OSC1_MOD2_SRC,
    PARAMETER_OSC1_MOD2_DEPTH,

    /* Oscillator 2 */

    PARAMETER_OSC2_OCTAVE,
    PARAMETER_OSC2_SEMITONE,
    PARAMETER_OSC2_FINE,
    PARAMETER_OSC2_WAVE,
    PARAMETER_OSC2_MOD1_SRC,
    PARAMETER_OSC2_MOD1_DEPTH,
    PARAMETER_OSC2_MOD2_SRC,
    PARAMETER_OSC2_MOD2_DEPTH,

    /* Oscillator 3 */

    PARAMETER_OSC3_OCTAVE,
    PARAMETER_OSC3_SEMITONE,
    PARAMETER_OSC3_FINE,
    PARAMETER_OSC3_WAVE,
    PARAMETER_OSC3_MOD1_SRC,
    PARAMETER_OSC3_MOD1_DEPTH,
    PARAMETER_OSC3_MOD2_SRC,
    PARAMETER_OSC3_MOD2_DEPTH,

    /* DCA 1 */

    PARAMETER_DCA1_LEVEL,
    PARAMETER_DCA1_OUTPUT,
    PARAMETER_DCA1_MOD1_SRC,
    PARAMETER_DCA1_MOD1_DEPTH,
    PARAMETER_DCA1_MOD2_SRC,
    PARAMETER_DCA1_MOD2_DEPTH,

    /* DCA 2 */

    PARAMETER_DCA2_LEVEL,
    PARAMETER_DCA2_OUTPUT,
    PARAMETER_DCA2_MOD1_SRC,
    PARAMETER_DCA2_MOD1_DEPTH,
    PARAMETER_DCA2_MOD2_SRC,
    PARAMETER_DCA2_MOD2_DEPTH,

    /* DCA 3 */

    PARAMETER_DCA3_LEVEL,
    PARAMETER_DCA3_OUTPUT,
    PARAMETER_DCA3_MOD1_SRC,
    PARAMETER_DCA3_MOD1_DEPTH,
    PARAMETER_DCA3_MOD2_SRC,
    PARAMETER_DCA3_MOD2_DEPTH,

    /* DCA 4 */

    PARAMETER_DCA4_ENV4_DEPTH,
    PARAMETER_DCA4_PAN,
    PARAMETER_DCA4_MOD_SRC,
    PARAMETER_DCA4_MOD_DEPTH,

    /* Filter */

    PARAMETER_FILTER_FREQUENCY,
    PARAMETER_FILTER_RESONANCE,
    PARAMETER_FILTER_KEYBOARD_TRACKING,
    PARAMETER_FILTER_MOD1_SRC,
    PARAMETER_FILTER_MOD1_DEPTH,
    PARAMETER_FILTER_MOD2_SRC,
    PARAMETER_FILTER_MOD2_DEPTH,

    /* Modes */

    PARAMETER_AMPLITUDE_MODULATION,
    PARAMETER_GLIDE,
    PARAMETER_MONO,
    PARAMETER_SYNC,
    PARAMETER_VOICE_RESTART,
    PARAMETER_ENVELOPE_RESTART,
    PARAMETER_OSCILLATOR_RESTART,
    PARAMETER_ENVELOPE_FULL_CYCLE,

    PARAMETER_COUNT
} Parameters;

typedef struct {
    gchar *filename, *name, *type;
    guchar parameters[PARAMETER_COUNT];
} Patch;

extern Patch *current_patch;

typedef struct {
    GtkWidget *statusbar;
    guint statusbar_context_id;
} Statusbar;

void update_statusbar(Statusbar *, const gchar *);

#endif /* !MAIN_H */
