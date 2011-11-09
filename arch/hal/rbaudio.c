/* $Id: rbaudio.c,v 1.1.1.1 2006/03/17 19:53:40 zicodxx Exp $ */
/*
 *
 * SDL CD Audio functions
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <sys/ioctl.h>
#include <linux/cdrom.h>
#endif

#include "pstypes.h"
#include "error.h"
#include "args.h"
#include "rbaudio.h"
#include "console.h"
#include "timer.h"


static int initialised = 0;

void RBAExit()
{
}

void RBAInit()
{
  initialised = 1;
}

int RBAEnabled()
{
  return initialised;
}

int RBAPlayTrack(int a)
{
  return -1;
}

void (*redbook_finished_hook)() = NULL;

void RBAStop()
{
  redbook_finished_hook = NULL;	// no calling the finished hook - stopped means stopped here
}

void RBAEjectDisk()
{
  initialised = 0;
}

void RBASetVolume(int volume)
{
}

void RBAPause()
{
}

int RBAResume()
{
  return 1;
}

int RBAPauseResume()
{
  return 0;
}

int RBAGetNumberOfTracks()
{
  return -1;
}

// check if we need to call the 'finished' hook
// needs to go in all event loops
// a real hook would be ideal, but is currently unsupported by SDL Audio CD
void RBACheckFinishedHook()
{
}

// plays tracks first through last, inclusive
int RBAPlayTracks(int first, int last, void (*hook_finished)(void))
{
  return 0;
}

// return the track number currently playing.  Useful if RBAPlayTracks()
// is called.  Returns 0 if no track playing, else track number
int RBAGetTrackNum()
{
  return 0;
}

int RBAPeekPlayStatus()
{
  return 0;
}

unsigned long RBAGetDiscID()
{
  return 0;
}

void RBAList(void)
{
}
