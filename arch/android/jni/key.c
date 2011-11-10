/* $Id: key.c,v 1.1.1.1 2006/03/17 19:53:40 zicodxx Exp $ */
/*
 *
 * SDL keyboard input support
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "ipc.h"

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
//volatile unsigned char 	keyd_last_pressed;
//volatile unsigned char 	keyd_last_released;
volatile unsigned char	keyd_pressed[256];
fix64			keyd_time_when_last_pressed;
unsigned char		unicode_frame_buffer[KEY_BUFFER_SIZE] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };

typedef struct keyboard	{
	ubyte state[256];
} keyboard;

static keyboard key_data;

typedef struct key_props {
	char *key_text;
	unsigned char ascii_value;
	eEventKey sym;
} key_props;

key_props key_properties[256] = {
{ "",       255,    -1                 }, // 0
{ "ESC",    255,    eEventKey_SELECT },
{ "1",      '1',    -1 },
{ "2",      '2',    -1 },
{ "3",      '3',    -1 },
{ "4",      '4',    -1 },
{ "5",      '5',    -1 },
{ "6",      '6',    -1 },
{ "7",      '7',    -1 },
{ "8",      '8',    -1 },
{ "9",      '9',    -1 }, // 10
{ "0",      '0',    -1 },
{ "-",      '-',    -1 },
{ "=",      '=',    -1 },
{ "BSPC",   255,    -1 },
{ "TAB",    255,    -1 },
{ "Q",      'q',    eEventKey_TRIANGLE },
{ "W",      'w',    -1 },
{ "E",      'e',    -1 },
{ "R",      'r',    -1 },
{ "T",      't',    eEventKey_CIRCLE }, // 20
{ "Y",      'y',    eEventKey_L1 },
{ "U",      'u',    -1 },
{ "I",      'i',    -1 },
{ "O",      'o',    -1 },
{ "P",      'p',    -1 },
{ "[",      '[',    -1 },
{ "]",      ']',    -1 },
{ "ENTER",  255,    eEventKey_START },
{ "LCTRL",  255,    eEventKey_CROSS },
{ "A",      'a',    eEventKey_R1 }, // 30
{ "S",      's',    -1 },
{ "D",      'd',    -1 },
{ "F",      'f',    -1 },
{ "G",      'g',    -1 },
{ "H",      'h',    -1 },
{ "J",      'j',    -1 },
{ "K",      'k',    -1 },
{ "L",      'l',    -1 },
{ ";",      ';',    -1 },
{ "'",      '\'',   -1 }, // 40
{ "`",      '`',    -1 },
{ "LSHFT",  255,    -1 },
{ "\\",     '\\',   -1 },
{ "Z",      'z',    -1 },
{ "X",      'x',    -1 },
{ "C",      'c',    -1 },
{ "V",      'v',    -1 },
{ "B",      'b',    -1 },
{ "N",      'n',    -1 },
{ "M",      'm',    eEventKey_Menu }, // 50
{ ",",      ',',    -1 },
{ ".",      '.',    -1 },
{ "/",      '/',    -1 },
{ "RSHFT",  255,    -1 },
{ "PAD*",   '*',    -1 },
{ "LALT",   255,    -1 },
{ "SPC",    ' ',    eEventKey_SQUARE },
{ "CPSLK",  255,    -1 },
{ "F1",     255,    -1 },
{ "F2",     255,    -1 }, // 60
{ "F3",     255,    -1 },
{ "F4",     255,    -1 },
{ "F5",     255,    -1 },
{ "F6",     255,    -1 },
{ "F7",     255,    -1 },
{ "F8",     255,    -1 },
{ "F9",     255,    -1 },
{ "F10",    255,    -1 },
{ "NMLCK",  255,    -1 },
{ "SCLK",   255,    -1 }, // 70
{ "PAD7",   255,    -1 },
{ "PAD8",   255,    -1 },
{ "PAD9",   255,    -1 },
{ "PAD-",   255,    -1 },
{ "PAD4",   255,    -1 },
{ "PAD5",   255,    -1 },
{ "PAD6",   255,    -1 },
{ "PAD+",   255,    -1 },
{ "PAD1",   255,    -1 },
{ "PAD2",   255,    -1 }, // 80
{ "PAD3",   255,    -1 },
{ "PAD0",   255,    -1 },
{ "PAD.",   255,    -1 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "F11",    255,    -1 },
{ "F12",    255,    -1 },
{ "",       255,    -1                 },
{ "",       255,    -1                 }, // 90
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "PAUSE",  255,    -1 },
{ "W0",     255,    -1 },
{ "W1",     255,    -1 },
{ "W2",     255,    -1 }, // 100
{ "W3",     255,    -1 },
{ "W4",     255,    -1 },
{ "W5",     255,    -1 },
{ "W6",     255,    -1 },
{ "W7",     255,    -1 },
{ "W8",     255,    -1 },
{ "W9",     255,    -1 },
{ "W10",    255,    -1 },
{ "W11",    255,    -1 },
{ "W12",    255,    -1 }, // 110
{ "W13",    255,    -1 },
{ "W14",    255,    -1 },
{ "W15",    255,    -1 },
{ "W16",    255,    -1 },
{ "W17",    255,    -1 },
{ "W18",    255,    -1 },
{ "W19",    255,    -1 },
{ "W20",    255,    -1 },
{ "W21",    255,    -1 },
{ "W22",    255,    -1 }, // 120
{ "W23",    255,    -1 },
{ "W24",    255,    -1 },
{ "W25",    255,    -1 },
{ "W26",    255,    -1 },
{ "W27",    255,    -1 },
{ "W28",    255,    -1 },
{ "W29",    255,    -1 },
{ "W30",    255,    -1 },
{ "W31",    255,    -1 },
{ "W32",    255,    -1 }, // 130
{ "W33",    255,    -1 },
{ "W34",    255,    -1 },
{ "W35",    255,    -1 },
{ "W36",    255,    -1 },
{ "W37",    255,    -1 },
{ "W38",    255,    -1 },
{ "W39",    255,    -1 },
{ "W40",    255,    -1 },
{ "W41",    255,    -1 },
{ "W42",    255,    -1 }, // 140
{ "W43",    255,    -1 },
{ "W44",    255,    -1 },
{ "W45",    255,    -1 },
{ "W46",    255,    -1 },
{ "W47",    255,    -1 },
{ "W48",    255,    -1 },
{ "W49",    255,    -1 },
{ "W50",    255,    -1 },
{ "W51",    255,    -1 },
{ "",       255,    -1                 }, // 150
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "PAD",    255,    -1 },
{ "RCTRL",  255,    -1 },
{ "LCMD",   255,    -1 },
{ "RCMD",   255,    -1 },
{ "",       255,    -1                 }, // 160
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 }, // 170
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 }, // 180
{ "PAD/",   255,    -1 },
{ "",       255,    -1                 },
{ "PRSCR",  255,    -1 },
{ "RALT",   255,    -1 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 }, // 190
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "",       255,    -1                 },
{ "HOME",   255,    -1 },
{ "UP",     255,    eEventKey_DPAD_UP }, // 200
{ "PGUP",   255,    -1 },
{ "",       255,    -1                 },
{ "LEFT",   255,    eEventKey_DPAD_LEFT },
{ "",       255,    -1                 },
{ "RIGHT",  255,    eEventKey_DPAD_RIGHT },
{ "",       255,    -1                 },
{ "END",    255,    -1 },
{ "DOWN",   255,    eEventKey_DPAD_DOWN },
{ "PGDN",   255,    -1 },
{ "INS",    255,    -1 }, // 210
{ "DEL",    255,    -1 },
{ "W52",    255,    -1 },
{ "W53",    255,    -1 },
{ "W54",    255,    -1 },
{ "W55",    255,    -1 },
{ "W56",    255,    -1 },
{ "W57",    255,    -1 },
{ "W58",    255,    -1 },
{ "W59",    255,    -1 },
{ "W60",    255,    -1 }, // 220
{ "W61",    255,    -1 },
{ "W62",    255,    -1 },
{ "W63",    255,    -1 },
{ "W64",    255,    -1 },
{ "W65",    255,    -1 },
{ "W66",    255,    -1 },
{ "W67",    255,    -1 },
{ "W68",    255,    -1 },
{ "W69",    255,    -1 },
{ "W70",    255,    -1 }, // 230
{ "W71",    255,    -1 },
{ "W72",    255,    -1 },
{ "W73",    255,    -1 },
{ "W74",    255,    -1 },
{ "W75",    255,    -1 },
{ "W76",    255,    -1 },
{ "W77",    255,    -1 },
{ "W78",    255,    -1 },
{ "W79",    255,    -1 },
{ "W80",    255,    -1 }, // 240
{ "W81",    255,    -1 },
{ "W82",    255,    -1 },
{ "W83",    255,    -1 },
{ "W84",    255,    -1 },
{ "W85",    255,    -1 },
{ "W86",    255,    -1 },
{ "W87",    255,    -1 },
{ "W88",    255,    -1 },
{ "W89",    255,    -1 },
{ "W90",    255,    -1 }, // 250
{ "W91",    255,    -1 },
{ "W92",    255,    -1 },
{ "W93",    255,    -1 },
{ "W94",    255,    -1 },
{ "W95",    255,    -1 }, // 255
};




typedef struct d_event_keycommand
{
	event_type	type;	// EVENT_KEY_COMMAND/RELEASE
	int			keycode;
} d_event_keycommand;

char *key_text[256];

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

void key_handler(IPCEvent_t *kevent)
{
  int keycode;
  int event_keysym=-1;
  int key_state;

	// Read SDLK symbol and state
  event_keysym = kevent->key.key;
  key_state = (kevent->key.state == 1)?1:0;

#if 0 // Text-input perhaps?
	// fill the unicode frame-related unicode buffer 
	if (key_state && kevent->keysym.unicode > 31 && kevent->keysym.unicode < 255)
	{
		int i = 0;
		for (i = 0; i < KEY_BUFFER_SIZE; i++)
			if (unicode_frame_buffer[i] == '\0')
			{
				unicode_frame_buffer[i] = kevent->keysym.unicode;
				break;
			}
	}
#endif

	//=====================================================
	for (keycode = 255; keycode > 0; keycode--)
		if (key_properties[keycode].sym == event_keysym)
			break;

	if (keycode == 0)
		return;

	/* 
	 * process the key if:
	 * - it's a valid key AND
	 * - if the keystate has changed OR
	 * - key state same as last one and game accepts key repeats but keep out mod/lock keys
	 */
	if (key_state != keyd_pressed[keycode] || (keyd_repeat && !key_ismodlck(keycode)))
	{
		d_event_keycommand event;

		// now update the key props
		if (key_state) {
		  //keyd_last_pressed = keycode;
			keyd_pressed[keycode] = key_data.state[keycode] = 1;
		} else {
			keyd_pressed[keycode] = key_data.state[keycode] = 0;
		}

		if ( keyd_pressed[KEY_LSHIFT] || keyd_pressed[KEY_RSHIFT])
			keycode |= KEY_SHIFTED;
		if ( keyd_pressed[KEY_LALT] || keyd_pressed[KEY_RALT])
			keycode |= KEY_ALTED;
		if ( keyd_pressed[KEY_LCTRL] || keyd_pressed[KEY_RCTRL])
			keycode |= KEY_CTRLED;
		if ( keyd_pressed[KEY_DELETE] )
			keycode |= KEY_DEBUGGED;
		if ( keyd_pressed[KEY_LMETA] || keyd_pressed[KEY_RMETA])
			keycode |= KEY_METAED;

		// We allowed the key to be added to the queue for now,
		// because there are still input loops without associated windows
		event.type = key_state?EVENT_KEY_COMMAND:EVENT_KEY_RELEASE;
		event.keycode = keycode;
		con_printf(CON_DEBUG, "Sending event %s: %s %s %s %s %s %s\n",
				(key_state)                  ? "EVENT_KEY_COMMAND": "EVENT_KEY_RELEASE",
				(keycode & KEY_METAED)	? "META" : "",
				(keycode & KEY_DEBUGGED)	? "DEBUG" : "",
				(keycode & KEY_CTRLED)	? "CTRL" : "",
				(keycode & KEY_ALTED)	? "ALT" : "",
				(keycode & KEY_SHIFTED)	? "SHIFT" : "",
				key_properties[keycode & 0xff].key_text
				);
		con_printf(CON_NORMAL,"Sending event.. type=%i, keycode=%i, state=%i\n",event.type,event.keycode);
		event_send((d_event *)&event);
	}
}

void key_close()
{
      Installed = 0;
}

void key_init()
{
	int i;
	
	if (Installed) return;

	Installed=1;
	//SDL_EnableUNICODE(1);
	key_toggle_repeat(1);

	keyd_time_when_last_pressed = timer_query();
	
	for(i=0; i<256; i++)
		key_text[i] = key_properties[i].key_text;
	  
	// Clear the keyboard array
	key_flush();
}

void key_flush()
{
#if 1
 	int i;
	//Uint8 *keystate = SDL_GetKeyState(NULL);

	if (!Installed)
		key_init();

	//Clear the unicode buffer
	for (i=0; i<KEY_BUFFER_SIZE; i++ )
		unicode_frame_buffer[i] = '\0';

	for (i=0; i<256; i++ )	{
#if 0
		if (key_ismodlck(i) == KEY_ISLCK && keystate[key_properties[i].sym] && !GameArg.CtlNoStickyKeys) // do not flush status of sticky keys
		{
			keyd_pressed[i] = 1;
			key_data.state[i] = 0;
		}
		else
#endif
		{
			keyd_pressed[i] = 0;
			key_data.state[i] = 1;
		}
	}
#endif
}

int event_key_get(d_event *event)
{
	Assert(event->type == EVENT_KEY_COMMAND || event->type == EVENT_KEY_RELEASE);
	return ((d_event_keycommand *)event)->keycode;
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
#if 0
	if (enable)
	{
		if (SDL_EnableKeyRepeat(KEY_REPEAT_DELAY, KEY_REPEAT_INTERVAL) == 0)
			keyd_repeat = 1;
	}
	else
	{
		SDL_EnableKeyRepeat(0, 0);
		keyd_repeat = 0;
	}
	key_flush();
#endif
}
