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

extern void key_handler(IPCEvent_t *event);
#if 0
extern void mouse_button_handler(SDL_MouseButtonEvent *mbe);
extern void mouse_motion_handler(SDL_MouseMotionEvent *mme);
extern void joy_button_handler(SDL_JoyButtonEvent *jbe);
extern void joy_hat_handler(SDL_JoyHatEvent *jhe);
extern int joy_axis_handler(SDL_JoyAxisEvent *jae);
#endif
extern void mouse_cursor_autohide();
extern void mouse_toggle_cursor(int activate);

static int initialised=0;

void event_poll()
{
  IPCEvent_t event;
  int clean_uniframe=1;
  window *wind = window_get_front();
  int idle = 1;
	
  // If the front window changes, exit this loop, otherwise unintended behavior can occur
  // like pressing 'Return' really fast at 'Difficulty Level' causing multiple games to be started
  while ((wind == window_get_front()) && (IPCEvent_Poll(&event) == 0) )
    {
      switch(event.type) {
      case eEventType_Key:
	if (clean_uniframe)
	  memset(unicode_frame_buffer,'\0',sizeof(unsigned char)*KEY_BUFFER_SIZE);
	clean_uniframe=0;
	key_handler(&event);
	idle = 0;
	break;
#if 0
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
	mouse_button_handler((SDL_MouseButtonEvent *)&event);
	idle = 0;
	break;
      case SDL_MOUSEMOTION:
	mouse_motion_handler((SDL_MouseMotionEvent *)&event);
	idle = 0;
	break;
      case SDL_JOYBUTTONDOWN:
      case SDL_JOYBUTTONUP:
	joy_button_handler((SDL_JoyButtonEvent *)&event);
	idle = 0;
	break;
      case SDL_JOYAXISMOTION:
	if (joy_axis_handler((SDL_JoyAxisEvent *)&event))
	  idle = 0;
	break;
      case SDL_JOYHATMOTION:
	joy_hat_handler((SDL_JoyHatEvent *)&event);
	idle = 0;
	break;
      case SDL_JOYBALLMOTION:
	break;
      case SDL_QUIT:
	{
	  d_event qevent = { EVENT_QUIT };
	  call_default_handler(&qevent);
	  idle = 0;
	} 
	break;
#endif
      }
    }

  // Send the idle event if there were no other events
  if (idle)
    {
      d_event ievent;
      
      ievent.type = EVENT_IDLE;
      event_send(&ievent);
    }
  
  mouse_cursor_autohide();
}

void event_flush()
{
	IPCEvent_t event;
	
	while( IPCEvent_Poll(&event));
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
#if 0
	if (activate_focus && GameCfg.Grabinput)
		SDL_WM_GrabInput(SDL_GRAB_ON);
	else
		SDL_WM_GrabInput(SDL_GRAB_OFF);
#endif
	mouse_toggle_cursor(!activate_focus);
}

