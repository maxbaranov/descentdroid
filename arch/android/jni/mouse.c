/*
 *
 * SDL mouse driver
 *
 */

#include <string.h>

#include "fix.h"
#include "timer.h"
#include "event.h"
#include "mouse.h"
#include "playsave.h"

static struct mouseinfo {
	ubyte  button_state[MOUSE_MAX_BUTTONS];
	int    delta_x, delta_y, delta_z, old_delta_x, old_delta_y;
	int    x,y,z;
	int    cursor_enabled;
	fix64  cursor_time;
} Mouse;

typedef struct d_event_mousebutton
{
	event_type type;
	int button;
} d_event_mousebutton;

typedef struct d_event_mouse_moved
{
	event_type	type;	// EVENT_MOUSE_MOVED
	short		dx, dy, dz;
} d_event_mouse_moved;

void mouse_init(void)
{
	memset(&Mouse,0,sizeof(Mouse));
}

void mouse_close(void)
{
}

void mouse_flush()	// clears all mice events...
{
}

//========================================================================
void mouse_get_pos( int *x, int *y, int *z )
{
  *x=0;
  *y=0;
  *z=0;
}

void mouse_get_delta( int *dx, int *dy, int *dz )
{
  *dx = 0;
  *dy = 0;
  *dz = 0;
}

void event_mouse_get_delta(d_event *event, int *dx, int *dy, int *dz)
{
	Assert(event->type == EVENT_MOUSE_MOVED);

	*dx = 0;
	*dy = 0;
	*dz = 0;
}

int event_mouse_get_button(d_event *event)
{
	Assert((event->type == EVENT_MOUSE_BUTTON_DOWN) || (event->type == EVENT_MOUSE_BUTTON_UP));
	return ((d_event_mousebutton *)event)->button;
}

int mouse_get_btns()
{
  return 0;
}

void mouse_toggle_cursor(int activate)
{
}

// If we want to display/hide cursor, do so if not already and also hide it automatically after some time.
void mouse_cursor_autohide()
{
}
