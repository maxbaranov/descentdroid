#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <digi_opensl.h>
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

#include <SLES/OpenSLES.h>
#include "SLES/OpenSLES_Android.h"

#define MAX_CHANNELS 16

struct audio_cb {
  void (* fn)(struct audio_cb *);
  void *opaque;
};

struct audio_mixer {
  SLObjectItf o;
  SLEnvironmentalReverbItf reverb;
};

struct audio_buffer {
  SLObjectItf o;
  SLPlayItf player;
  SLAndroidSimpleBufferQueueItf queue;
  //  SLEffectSendItf effect;
  SLVolumeItf volume;
  int isPlaying;
  short *data;
  int length;
  void (* done)(struct audio_buffer *);

  int d_soundnum;

  //struct audio_buffer *next;
};

struct audio_engine {
  SLObjectItf o;
  SLEngineItf engine;
  struct audio_mixer mixer;
  struct audio_buffer buffer[MAX_CHANNELS];
  void (* done)(struct audio_buffer *);
};

static struct audio_engine ae;

static void digi_opensl_done(struct audio_buffer *b) {
}

static void buffer_finished(SLAndroidSimpleBufferQueueItf bq, void *context) {
  struct audio_buffer *b = (struct audio_buffer *)context;

  // TODO: Check if it's supposed to be looping

 
  if(b->done)
    (*b->done)(b);

  b->isPlaying = 0;
}

static int digi_opensl_buffer_create(struct audio_engine *e, int bufferindex) {
  SLDataLocator_AndroidSimpleBufferQueue loc_bufq = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 8 };
  SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, e->mixer.o };
  SLDataSink audioSnk = { &loc_outmix, NULL };
  SLDataFormat_PCM format_pcm = {
    .formatType     = SL_DATAFORMAT_PCM,
    .numChannels    = 2,
    .samplesPerSec  = SL_SAMPLINGRATE_11_025,
    //.bitsPerSample  = SL_PCMSAMPLEFORMAT_FIXED_16,
    //.containerSize  = SL_PCMSAMPLEFORMAT_FIXED_16,
    .bitsPerSample  = SL_PCMSAMPLEFORMAT_FIXED_8,
    .containerSize  = SL_PCMSAMPLEFORMAT_FIXED_8,
    .channelMask    = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
    .endianness     = SL_BYTEORDER_LITTLEENDIAN
  };
  SLDataSource audioSrc = { &loc_bufq, &format_pcm };
  SLresult result;
  struct audio_buffer *buff;

  buff = &e->buffer[bufferindex];

  // configure audio sink

  // create audio player
  const SLInterfaceID ids[2] = { SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND };
  const SLboolean req[2] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

  result = (*e->engine)->CreateAudioPlayer(e->engine, &buff->o, &audioSrc, &audioSnk, 2, ids, req);
  if (result != SL_RESULT_SUCCESS)
    return -1;

  // realize the player
  result = (*buff->o)->Realize(buff->o, SL_BOOLEAN_FALSE);
  if (result != SL_RESULT_SUCCESS)
    return -1;

  // get the play interface
  result = (*buff->o)->GetInterface(buff->o, SL_IID_PLAY, &buff->player);
  if (result != SL_RESULT_SUCCESS)
    return -1;

  // get the buffer queue interface
  result = (*buff->o)->GetInterface(buff->o, SL_IID_BUFFERQUEUE, &buff->queue);
  if (result != SL_RESULT_SUCCESS)
    return -1;

  // register callback on the buffer queue
  result = (*buff->queue)->RegisterCallback(buff->queue, buffer_finished, buff);
  if (result != SL_RESULT_SUCCESS)
    return -1;

  // get the volume send interface
  result = (*buff->o)->GetInterface(buff->o, SL_IID_VOLUME, &buff->volume);
  if (result != SL_RESULT_SUCCESS) {
    //return -1;
    con_printf(CON_NORMAL,"Buffer %i has no Volume Interface\n",bufferindex);
    buff->volume = NULL;
  }

  // set the player's state to playing
  result = (*buff->player)->SetPlayState(buff->player, SL_PLAYSTATE_PLAYING);
  if (result != SL_RESULT_SUCCESS)
    return -1;

  buff->done = e->done;

  return 0;
}

/* Initialise audio devices. */
int digi_opensl_init() {
  SLresult result;
  int i;

  memset(&ae, 0, sizeof(ae));

  result = slCreateEngine(&ae.o, 0, NULL, 0, NULL, NULL);
  if (result != SL_RESULT_SUCCESS)
    return -1;

  result = (*ae.o)->Realize(ae.o, SL_BOOLEAN_FALSE);
  if (result != SL_RESULT_SUCCESS)
    return -1;

  result = (*ae.o)->GetInterface(ae.o, SL_IID_ENGINE, &ae.engine);
  if (result != SL_RESULT_SUCCESS)
    return -1;

  // create output mix, with environmental reverb specified as a
  // non-required interface
  const SLInterfaceID ids[1] = { SL_IID_ENVIRONMENTALREVERB };
  const SLboolean req[1] = { SL_BOOLEAN_FALSE };
  result = (*ae.engine)->CreateOutputMix(ae.engine, &ae.mixer.o, 0, ids, req);
  if (result != SL_RESULT_SUCCESS)
    return -1;

  result = (*ae.mixer.o)->Realize(ae.mixer.o, SL_BOOLEAN_FALSE);
  if (result != SL_RESULT_SUCCESS)
    return -1;

  ae.done = digi_opensl_done;

  for(i=0;i<MAX_CHANNELS;i++) {
    digi_opensl_buffer_create(&ae,i);
  }

  return 0;
}

/* Toggle audio */
void digi_opensl_reset() { }

/* Shut down audio */
void digi_opensl_close() {
  struct audio_buffer *b, *lb;
  int i;

  for(i=0;i<MAX_CHANNELS;i++) {
    (*ae.buffer[i].o)->Destroy( ae.buffer[i].o );
  }

  if(ae.mixer.o != NULL)
    (*ae.mixer.o)->Destroy(ae.mixer.o);
  if(ae.o != NULL)
    (*ae.o)->Destroy(ae.o);

  memset(&ae, 0, sizeof(ae));
}

void digi_opensl_stop_all_channels()
{
  con_printf(CON_NORMAL,"Should have stopped all audio here\n");
}


// Volume 0-F1_0
int digi_opensl_start_sound(short soundnum, fix volume, int pan, int looping, int loop_start, int loop_end, int soundobj) {
  int i;

  if(soundnum < 0)
    return -1;

  Assert(GameSounds[soundnum].data != (void *)-1);

  for(i=0;i<MAX_CHANNELS;i++) {

    if( !ae.buffer[i].isPlaying ) {
      struct audio_buffer *b;

      b = &ae.buffer[i];
      b->isPlaying = 1;
      int iMBVol;

      iMBVol = 0x10000 - volume;

      //(*b->volume)->SetVolumeLevel(b->volume,iMBVol);
      (*b->queue)->Enqueue(b->queue,GameSounds[soundnum].data,GameSounds[soundnum].length);
    }
  }
  
  return 0;
}

// Returns the channel a sound number is playing on, or
// -1 if none.
int digi_opensl_find_channel(int soundno)
{
  return -1;
}


void digi_opensl_set_digi_volume( int dvolume )
{
}


int digi_opensl_is_sound_playing(int soundno) {
  int i;

  for(i=0;i<MAX_CHANNELS;i++) {
    if( ae.buffer[i].d_soundnum = soundno ) return 1;
  }

  return 0;
}


void digi_opensl_set_max_channels(int n) {
}

int digi_opensl_get_max_channels() {
  return MAX_CHANNELS;
}

int digi_opensl_is_channel_playing(int channel) {
  return 0;
}

void digi_opensl_set_channel_volume(int channel, int volume)
{
}

void digi_opensl_set_channel_pan(int channel, int pan)
{
}

void digi_opensl_stop_sound(int channel)
{
}

void digi_opensl_end_sound(int channel)
{
}

