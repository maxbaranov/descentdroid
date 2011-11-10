// Holds the main init and de-init functions for arch-related program parts

#include "songs.h"
#include "key.h"
#include "digi.h"
#include "mouse.h"
#include "joy.h"
#include "gr.h"
#include "error.h"
#include "text.h"
#include "args.h"
#include "config.h"

void arch_close(void)
{
	songs_uninit();

	gr_close();

	if (!GameArg.CtlNoJoystick)
		joy_close();

	mouse_close();

	if (!GameArg.SndNoSound)
	{
		digi_close();
	}

	key_close();
}

void arch_init(void)
{
	int t;

	key_init();

	//digi_select_system(0); // NULL
	digi_select_system(1);  // OpenSL

	if (!GameArg.SndNoSound)
		digi_init();

	mouse_init();

	if (!GameArg.CtlNoJoystick)
		joy_init();

	if ((t = gr_init(0)) != 0)
		Error(TXT_CANT_INIT_GFX,t);

	atexit(arch_close);
}

