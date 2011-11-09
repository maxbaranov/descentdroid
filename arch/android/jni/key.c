/* $Id: key.c,v 1.1.1.1 2006/03/17 19:53:40 zicodxx Exp $ */
/*
 *
 * SDL keyboard input support
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "event.h"
#include "error.h"
#include "key.h"
#include "timer.h"
#include "window.h"
#include "console.h"
#include "args.h"

static unsigned char Installed = 0;

//-------- Variable accessed by outside functions ---------
int			keyd_repeat = 0; // 1 = use repeats, 0 no repeats
volatile unsigned char 	keyd_last_pressed;
volatile unsigned char 	keyd_last_released;
volatile unsigned char	keyd_pressed[256];
fix64			keyd_time_when_last_pressed;
unsigned char		unicode_frame_buffer[KEY_BUFFER_SIZE] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };

typedef struct keyboard	{
	ubyte state[256];
} keyboard;

static keyboard key_data;

typedef struct d_event_keycommand
{
	event_type	type;	// EVENT_KEY_COMMAND/RELEASE
	int			keycode;
} d_event_keycommand;

char *key_text[256];

void key_setkeyd_pressed(int ndx,unsigned char val) {
  keyd_pressed[ndx] = val;
}

int key_ismodlck(int keycode)
{
	switch (keycode)
	{
		case KEY_LSHIFT:
		case KEY_RSHIFT:
		case KEY_LALT:
		case KEY_RALT:
		case KEY_LCTRL:
		case KEY_RCTRL:
		case KEY_LMETA:
		case KEY_RMETA:
			return KEY_ISMOD;
		case KEY_NUMLOCK:
		case KEY_SCROLLOCK:
		case KEY_CAPSLOCK:
			return KEY_ISLCK;
		default:
			return 0;
	}
}

unsigned char key_ascii()
{
	static unsigned char unibuffer[KEY_BUFFER_SIZE] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
	int i=0, offset=0, count=0;
	
	offset=strlen((const char*)unibuffer);

	// move temporal chars from unicode_frame_buffer to empty space behind last unibuffer char (if any)
	for (i=offset; i < KEY_BUFFER_SIZE; i++)
		if (unicode_frame_buffer[count] != '\0')
		{
			unibuffer[i]=unicode_frame_buffer[count];
			unicode_frame_buffer[count]='\0';
			count++;
		}

	// unibuffer is not empty. store first char, remove it, shift all chars one step left and then print our char
	if (unibuffer[0] != '\0')
	{
		unsigned char retval = unibuffer[0];
		unsigned char unibuffer_shift[KEY_BUFFER_SIZE];
		memset(unibuffer_shift,'\0',sizeof(unsigned char)*KEY_BUFFER_SIZE);
		memcpy(unibuffer_shift,unibuffer+1,sizeof(unsigned char)*(KEY_BUFFER_SIZE-1));
		memcpy(unibuffer,unibuffer_shift,sizeof(unsigned char)*KEY_BUFFER_SIZE);
		return retval;
	}
	else
		return 255;
}

void key_close()
{
      Installed = 0;
}

void key_init()
{
	if (Installed) return;
	Installed=1;

	key_toggle_repeat(1);

	keyd_time_when_last_pressed = timer_query();
	
	// Clear the keyboard array
	key_flush();
}

void key_flush()
{
}

int event_key_get(d_event *event)
{
	Assert(event->type == EVENT_KEY_COMMAND || event->type == EVENT_KEY_RELEASE);
	return 0;
}

// same as above but without mod states
int event_key_get_raw(d_event *event)
{
	int keycode = ((d_event_keycommand *)event)->keycode;
	Assert(event->type == EVENT_KEY_COMMAND || event->type == EVENT_KEY_RELEASE);
	if ( keycode & KEY_SHIFTED ) keycode &= ~KEY_SHIFTED;
	if ( keycode & KEY_ALTED ) keycode &= ~KEY_ALTED;
	if ( keycode & KEY_CTRLED ) keycode &= ~KEY_CTRLED;
	if ( keycode & KEY_DEBUGGED ) keycode &= ~KEY_DEBUGGED;
	if ( keycode & KEY_METAED ) keycode &= ~KEY_METAED;
	return keycode;
}

void key_toggle_repeat(int enable)
{
	key_flush();
}
