/* $Id: event.c,v 1.1.1.1 2006/03/17 19:53:40 zicodxx Exp $ */
/*
 *
 * SDL Event related stuff
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "key.h"
#include "window.h"
#include "timer.h"
#include "main/config.h"
#include "ipc.h"

static int initialised=0;

typedef struct d_event_keycommand
{
        event_type      type;   // EVENT_KEY_COMMAND/RELEASE
        int                     keycode;
} d_event_keycommand;

void key_setkeyd_pressed(int ndx,unsigned char val);

void event_poll() {
  IPCEvent_t ev;
  d_event_keycommand descentevent;
  window *wind = window_get_front();
  int bIdle = 1;

  while(wind == window_get_front() && IPCEvent_Poll(&ev) == 0 ) {
    if( ev.key.state == 1 )
      descentevent.type = EVENT_KEY_COMMAND;
    else
      descentevent.type = EVENT_KEY_RELEASE;
    
    if(      ev.key.key == eEventKey_L1 ) descentevent.keycode = 1;  // ESC
    else if( ev.key.key == eEventKey_R1 ) descentevent.keycode = 28; // ENTER

    key_setkeyd_pressed(descentevent.keycode,ev.key.state);

    event_send((d_event*)&descentevent);

    con_printf(CON_NORMAL,"Sending event.. type=%i, keycode=%i\n",descentevent.type,descentevent.keycode);
    bIdle = 0;
  }
   
  if( bIdle ) {
    d_event ievent;

    //con_printf(CON_NORMAL,"No events...\n");
                
    ievent.type = EVENT_IDLE;
    event_send(&ievent);
  }

}

void event_flush() {
  IPCEvent_t ev;

  while(IPCEvent_Poll(&ev) == 0 ) { }
}

int event_init()
{
  // We should now be active and responding to events.
  initialised = 1;

  return 0;
}

int (*default_handler)(d_event *event) = NULL;

void set_default_handler(int (*handler)(d_event *event))
{
  default_handler = handler;
}

int call_default_handler(d_event *event)
{
  if (default_handler)
    return (*default_handler)(event);
	
  return 0;
}

void event_send(d_event *event)
{
	window *wind;

	if ((wind = window_get_front()))
	{
		if (!window_send_event(wind, event))
			call_default_handler(event);
	}
	else
		call_default_handler(event);
}

// Process the first event in queue, sending to the appropriate handler
// This is the new object-oriented system
// Uses the old system for now, but this may change
void event_process(void)
{
	d_event event;
	window *wind = window_get_front();

	timer_update();

	event_poll();	// send input events first

	// Doing this prevents problems when a draw event can create a newmenu,
	// such as some network menus when they report a problem
	if (window_get_front() != wind)
		return;
	
	event.type = EVENT_WINDOW_DRAW;	// then draw all visible windows
	wind = window_get_first();
	while (wind != NULL)
	{
		window *prev = window_get_prev(wind);
		if (window_is_visible(wind))
			window_send_event(wind, &event);
		if (!window_exists(wind))
		{
			if (!prev) // well there isn't a previous window ...
				break; // ... just bail out - we've done everything for this frame we can.
			wind = window_get_next(prev); // the current window seemed to be closed. so take the next one from the previous which should be able to point to the one after the current closed
		}
		else
			wind = window_get_next(wind);
	}

	gr_flip();
}

void event_toggle_focus(int activate_focus)
{
}

