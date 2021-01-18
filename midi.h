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

#ifndef MIDI_H
#define MIDI_H

typedef struct {
    int id;
    char *device;
    char *name;
} MIDIDevice;

typedef struct {
    unsigned char status;
    unsigned char data1;
    unsigned char data2;
} MIDIMessage;

int midi_initialise(void);
int midi_get_device_count(void);
MIDIDevice **midi_get_devices(void);

int midi_open(MIDIDevice *);
int midi_close(void);

int midi_note_on(unsigned char, unsigned char, unsigned char);
int midi_note_off(unsigned char, unsigned char, unsigned char);
int midi_program_change(unsigned char, unsigned char);
int midi_write(MIDIMessage *, unsigned);

#endif /* !MIDI_H */
