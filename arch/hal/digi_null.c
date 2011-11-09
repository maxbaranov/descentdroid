#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <digi_null.h>
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
#include "config.h"

int digi_max_channels = 16;

/* Initialise audio devices. */
int digi_null_init()
{
  return 0;
}

/* Toggle audio */
void digi_null_reset() { }

/* Shut down audio */
void digi_null_close()
{
}

void digi_null_stop_all_channels()
{
}


// Volume 0-F1_0
int digi_null_start_sound(short soundnum, fix volume, int pan, int looping, int loop_start, int loop_end, int soundobj)
{
  return 0;
}

// Returns the channel a sound number is playing on, or
// -1 if none.
int digi_null_find_channel(int soundno)
{
  return -1;
}


void digi_null_set_digi_volume( int dvolume )
{
}


int digi_null_is_sound_playing(int soundno)
{
  return 0;
}


void digi_null_set_max_channels(int n) {
}

int digi_null_get_max_channels() {
  return 16;
}

int digi_null_is_channel_playing(int channel)
{
  return 0;
}

void digi_null_set_channel_volume(int channel, int volume)
{
}

void digi_null_set_channel_pan(int channel, int pan)
{
}

void digi_null_stop_sound(int channel)
{
}

void digi_null_end_sound(int channel)
{
}

