/*
 * Digital audio support
 * Library-independent stub for dynamic selection of sound system
 */

#include "pstypes.h"
#include "error.h"
#include "fix.h"
#include "vecmat.h"
#include "gr.h"
#include "piggy.h"
#include "digi.h"
#include "sounds.h"
#include "wall.h"
#include "newdemo.h"
#include "kconfig.h"
#include "console.h"
#include "rbaudio.h"
#include "jukebox.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "digi.h"
#include "digi_null.h"

/* Sound system function pointers */

int  (*fptr_init)() = NULL;
void (*fptr_close)() = NULL;
void (*fptr_reset)() = NULL;

void (*fptr_set_channel_volume)(int, int) = NULL;
void (*fptr_set_channel_pan)(int, int) = NULL;

int  (*fptr_start_sound)(short, fix, int, int, int, int, int) = NULL;
void (*fptr_stop_sound)(int) = NULL;
void (*fptr_end_sound)(int) = NULL;

int  (*fptr_is_sound_playing)(int) = NULL;
int  (*fptr_is_channel_playing)(int) = NULL;
void (*fptr_stop_all_channels)() = NULL;
void (*fptr_set_digi_volume)(int) = NULL;

void digi_select_system(int n) {
	switch (n) {
	case SDLAUDIO_SYSTEM:
	default:
	con_printf(CON_NORMAL,"Using NULL audio\n");
        fptr_init = digi_null_init;
        fptr_close = digi_null_close;
        fptr_reset = digi_null_reset;
        fptr_set_channel_volume = digi_null_set_channel_volume;
        fptr_set_channel_pan = digi_null_set_channel_pan;
        fptr_start_sound = digi_null_start_sound;
        fptr_stop_sound = digi_null_stop_sound;
        fptr_end_sound = digi_null_end_sound;
        fptr_is_sound_playing = digi_null_is_sound_playing;
        fptr_is_channel_playing = digi_null_is_channel_playing;
        fptr_stop_all_channels = digi_null_stop_all_channels;
	fptr_set_digi_volume = digi_null_set_digi_volume;
 	break;
	}
}

/* Common digi functions */
extern int digi_max_channels;
int digi_sample_rate = SAMPLE_RATE_11K;
int digi_volume = SOUND_MAX_VOLUME;

void digi_set_volume(int dvolume) {
	digi_volume = dvolume;
	if (fptr_set_digi_volume) digi_set_digi_volume(dvolume);
}

void digi_set_sample_rate(int r) { digi_sample_rate = r; }

/* Stub functions */

int  digi_init()
{
	digi_init_sounds();
	return fptr_init();
}

void digi_close() { fptr_close(); }
void digi_reset() { fptr_reset(); }

void digi_set_channel_volume(int channel, int volume) { fptr_set_channel_volume(channel, volume); }
void digi_set_channel_pan(int channel, int pan) { fptr_set_channel_pan(channel, pan); }

int  digi_start_sound(short soundnum, fix volume, int pan, int looping, int loop_start, int loop_end, int soundobj) { return fptr_start_sound(soundnum, volume, pan, looping, loop_start, loop_end, soundobj); }
void digi_stop_sound(int channel) { fptr_stop_sound(channel); }
void digi_end_sound(int channel) { fptr_end_sound(channel); }

int  digi_is_sound_playing(int soundno) { return fptr_is_sound_playing(soundno); }
int  digi_is_channel_playing(int channel) { return fptr_is_channel_playing(channel); }
void digi_stop_all_channels() { fptr_stop_all_channels(); }
void digi_set_digi_volume(int dvolume) { fptr_set_digi_volume(dvolume); }


