#ifndef __DIGI_OPENSL__
#define __DIGI_OPENSL__

#include "fix.h"

int  digi_opensl_init();
void digi_opensl_reset();
void digi_opensl_close();
void digi_opensl_stop_all_channels();
int  digi_opensl_start_sound(short, fix, int, int, int, int, int );
int  digi_opensl_is_sound_playing(int );
int  digi_opensl_is_channel_playing(int );
void digi_opensl_set_channel_volume(int, int );
void digi_opensl_set_channel_pan(int, int );
void digi_opensl_stop_sound(int );
void digi_opensl_end_sound(int );
void digi_opensl_set_digi_volume(int);
void digi_opensl_debug();
int  digi_opensl_get_max_channels();

#endif
