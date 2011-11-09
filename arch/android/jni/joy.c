/*
 *
 * SDL joystick support
 *
 */

#include <string.h>   // for memset

#include "joy.h"
#include "error.h"
#include "timer.h"
#include "console.h"
#include "event.h"
#include "text.h"
#include "u_mem.h"
#include "playsave.h"

int num_joysticks = 0;
int joy_num_axes = 0;



/* ----------------------------------------------- */

void joy_init()
{
}

void joy_close()
{
}

void event_joystick_get_axis(d_event *event, int *axis, int *value)
{
	Assert(event->type == EVENT_JOYSTICK_MOVED);
}

void joy_flush()
{
}

int event_joystick_get_button(d_event *event)
{
}
