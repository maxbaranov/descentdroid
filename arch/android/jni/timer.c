/*
 *
 * SDL library timer functions
 *
 */

#include <sys/time.h>

#include "maths.h"
#include "timer.h"
#include "main/config.h"

static fix64 F64_RunTime = 0;

unsigned long long starttime = 0;

unsigned long long OSAL_GetTimeMS(void)
{
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (unsigned int)((tv.tv_sec % 0x3fffff)*1000 + tv.tv_usec/1000);
}

void timer_update(void)
{
  unsigned long long currtime;

  if( starttime == 0 )
    starttime = OSAL_GetTimeMS();

  currtime = OSAL_GetTimeMS();

  F64_RunTime = currtime - starttime;
}

fix64 timer_query(void)
{
  return (F64_RunTime);
}

void timer_delay(fix seconds)
{
  usleep( f2i(fixmul(seconds, i2f(1000))) * 1000 );
}

// Replacement for timer_delay which considers calc time the program needs between frames (not reentrant)
void timer_delay2(int fps)
{
	static u_int32_t FrameStart=0;
	u_int32_t FrameLoop=0;

	while (FrameLoop < 1000/(GameCfg.VSync?MAXIMUM_FPS:fps))
	{
	  u_int32_t tv_now = OSAL_GetTimeMS();
		if (FrameStart > tv_now)
			FrameStart = tv_now;
		if (!GameCfg.VSync)
		  usleep(100);
		FrameLoop=tv_now-FrameStart;
	}

	FrameStart=OSAL_GetTimeMS();
}
