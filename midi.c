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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <portmidi.h>

#include "midi.h"

static int midi_device_count = 0;
static MIDIDevice **midi_devices = NULL;
static PortMidiStream *output = NULL;

/**
   \brief Initialises MIDI support by building a list of available MIDI devices.

    \return one on success, zero on error or if no MIDI devices can be found.
 */
int
midi_initialise(void)
{
    PmError err = Pm_Initialize();

    if (err != pmNoError) {
        fprintf(stderr, "Unable to initialise MIDI: %s\n", Pm_GetErrorText(err));
        return 0;
    }

    int count = Pm_CountDevices();

    if (count == 0) {
        fprintf(stderr, "Unable to initialise MIDI: no devices found\n");
        return 0;
    }

    midi_devices = calloc(count + 1, sizeof(MIDIDevice *));

    for (int i = 0; i < count; ++i) {
        const PmDeviceInfo *info  = Pm_GetDeviceInfo(i);
        if (info->output) {
            midi_devices[midi_device_count] = malloc(sizeof(MIDIDevice));
            midi_devices[midi_device_count]->id = i;
            midi_devices[midi_device_count]->device = strdup(info->interf);
            midi_devices[midi_device_count]->name = strdup(info->name);
            midi_device_count++;
        }
    }

    if (midi_device_count < 1) {
        free(midi_devices);
        fprintf(stderr, "Unable to initialise MIDI: no usable devices found\n");
        return 0;
    }

    return 1;
}

/**
   \brief Returns the number of available MIDI interface devices.

   \return the number of available MIDI interface devices.
 */
int
midi_get_device_count(void)
{
    if (midi_device_count < 1) {
        fprintf(stderr, "MIDI not initialised\n");
    }

    return midi_device_count;
}

/**
   \brief Returns a list of available MIDI interface devices.

   \return the NULL terminated list of available MIDI interface devices.
 */
MIDIDevice **
midi_get_devices(void)
{
    if (midi_device_count < 1) {
        fprintf(stderr, "MIDI not initialised\n");
    }

    return midi_devices;
}

/**
   \brief Open a MIDI interface device.

   \param device - filename of the MIDI interface device to open.
   \return one on success, zero on error.
 */
int
midi_open(MIDIDevice *device)
{
    if (output != NULL) {
        fprintf(stderr, "A MIDI device is already open\n");
        return 0;
    }

    PmError err = Pm_OpenOutput(&output, device->id, NULL, 0, NULL, NULL, 0);

    if (err != pmNoError) {
        fprintf(stderr, "Unable to open MIDI device: %s\n", Pm_GetErrorText(err));
        return 0;
    }

    return 1;
}

/**
   \brief Close a MIDI interface device.

   \return one on success, zero on error.
 */
int
midi_close()
{
    if (output == NULL) {
        fprintf(stderr, "MIDI device is not open\n");
        return 0;
    }

    PmError err = Pm_Close(output);

    if (err != pmNoError) {
        fprintf(stderr, "Unable to close MIDI device: %s\n", Pm_GetErrorText(err));
        return 0;
    }

    output = NULL;

    return 1;
}

/**
   \brief Transmit a MIDI Note On message.

   \param channel - MIDI channel.
   \param note - note number.
   \param velocity - note velocity.
   \return one on success, zero on error.
 */
int
midi_note_on(unsigned char channel, unsigned char note, unsigned char velocity)
{
    if (output == NULL) {
        fprintf(stderr, "MIDI device is not open\n");
        return 0;
    }

    PmError err = Pm_WriteShort(output, 0, Pm_Message(0x90 | channel, note & 0x7f, velocity & 0x7f));

    if (err != pmNoError) {
        fprintf(stderr, "Unable send note on: %s\n", Pm_GetErrorText(err));
        return 0;
    }

    return 1;
}

/**
   \brief Transmit a MIDI Note Off message.

   \param channel - MIDI channel.
   \param note - note number.
   \param velocity - note velocity.
   \return one on success, zero on error.
 */
int
midi_note_off(unsigned char channel, unsigned char note, unsigned char velocity)
{
    if (output == NULL) {
        fprintf(stderr, "MIDI device is not open\n");
        return 0;
    }

    PmError err = Pm_WriteShort(output, 0, Pm_Message(0x80 | channel, note & 0x7f, velocity & 0x7f));

    if (err != pmNoError) {
        fprintf(stderr, "Unable send note off: %s\n", Pm_GetErrorText(err));
        return 0;
    }

    return 1;
}

/**
   \brief Transmit a MIDI Program Change message.

   \param channel - MIDI channel.
   \param program - program number.
   \return one on success, zero on error.
 */
int
midi_program_change(unsigned char channel, unsigned char program)
{
    if (output == NULL) {
        fprintf(stderr, "MIDI device is not open\n");
        return 0;
    }

    PmError err = Pm_WriteShort(output, 0, Pm_Message(0xc0 | channel, program & 0x7f, 0));

    if (err != pmNoError) {
        fprintf(stderr, "Unable send program change: %s\n", Pm_GetErrorText(err));
        return 0;
    }

    return 1;
}

/**
   \brief Write messages to a MIDI interface device.

   \param messages - the messages to write.
   \param count - the number of messages to write.
   \return one on success, zero on error.
 */
int
midi_write(MIDIMessage *messages, unsigned count)
{
    if (output == NULL) {
        fprintf(stderr, "MIDI device is not open\n");
        return 0;
    }

    for (int i = 0; i < count; ++i) {
        PmError err = Pm_WriteShort(output, 0, Pm_Message(messages[i].status, messages[i].data1, messages[i].data2));

        if (err != pmNoError) {
            fprintf(stderr, "Unable send messages: %s\n", Pm_GetErrorText(err));
            return 0;
        }
    }

    return 1;
}
