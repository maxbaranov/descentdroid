#ifndef __DIGI_NULL__
#define __DIGI_NULL__

#include "fix.h"

int  digi_null_init();
void digi_null_reset();
void digi_null_close();
void digi_null_stop_all_channels();
int  digi_null_start_sound(short, fix, int, int, int, int, int );
int  digi_null_is_sound_playing(int );
int  digi_null_is_channel_playing(int );
void digi_null_set_channel_volume(int, int );
void digi_null_set_channel_pan(int, int );
void digi_null_stop_sound(int );
void digi_null_end_sound(int );
void digi_null_set_digi_volume(int);
void digi_null_debug();

#endif
