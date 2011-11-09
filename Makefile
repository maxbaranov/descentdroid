CC := gcc
AR := ar
CFLAGS := -g -Wall -DNETWORK -DOGL -I. -Iinclude/ -Imain/ -Iarch/include/
LDFLAGS := -g -lm -lGL -lGLU -lSDL

OBJS :=

OBJS += 2d/2dsline.o \
  2d/bitblt.o \
  2d/bitmap.o \
  2d/box.o \
  2d/canvas.o \
  2d/circle.o \
  2d/disc.o \
  2d/font.o \
  2d/gpixel.o \
  2d/line.o \
  2d/palette.o \
  2d/pcx.o \
  2d/pixel.o \
  2d/poly.o \
  2d/rect.o \
  2d/rle.o \
  2d/scalec.o \
  3d/clipper.o \
  3d/draw.o \
  3d/globvars.o \
  3d/instance.o \
  3d/interp.o \
  3d/matrix.o \
  3d/points.o \
  3d/rod.o \
  3d/setup.o \
  iff/iff.o \
  main/ai.o \
  main/aipath.o \
  main/automap.o \
  main/bm.o \
  main/bmread.o \
  main/cntrlcen.o \
  main/collide.o \
  main/config.o \
  main/console.o \
  main/controls.o \
  main/credits.o \
  main/custom.o \
  main/digiobj.o \
  main/dumpmine.o \
  main/effects.o \
  main/endlevel.o \
  main/fireball.o \
  main/fuelcen.o \
  main/fvi.o \
  main/game.o \
  main/gamecntl.o \
  main/gamefont.o \
  main/gamemine.o \
  main/gamerend.o \
  main/gamesave.o \
  main/gameseg.o \
  main/gameseq.o \
  main/gauges.o \
  main/hash.o \
  main/hostage.o \
  main/hud.o \
  main/inferno.o \
  main/kconfig.o \
  main/kmatrix.o \
  main/laser.o \
  main/lighting.o \
  main/menu.o \
  main/mglobal.o \
  main/mission.o \
  main/morph.o \
  main/newdemo.o \
  main/newmenu.o \
  main/object.o \
  main/paging.o \
  main/physics.o \
  main/piggy.o \
  main/player.o \
  main/playsave.o \
  main/polyobj.o \
  main/powerup.o \
  main/render.o \
  main/robot.o \
  main/scores.o \
  main/slew.o \
  main/snddecom.o \
  main/songs.o \
  main/state.o \
  main/switch.o \
  main/terrain.o \
  main/texmerge.o \
  main/text.o \
  main/titles.o \
  main/vclip.o \
  main/wall.o \
  main/weapon.o \
  main/multi.o \
  maths/fixc.o \
  maths/rand.o \
  maths/tables.o \
  maths/vecmat.o \
  mem/mem.o \
  misc/args.o \
  misc/dl_list.o \
  misc/error.o \
  misc/hmp.o \
  misc/ignorecase.o \
  misc/physfsx.o \
  misc/strio.o \
  misc/strutil.o \
  texmap/ntmap.o \
  texmap/scanline.o \
  main/multibot.o \
  arch/hal/digi_null.o \
  arch/hal/digi.o \
  arch/hal/event.o \
  arch/hal/gr.o \
  arch/hal/ogl.o \
  arch/hal/init.o \
  arch/hal/joy.o \
  arch/hal/key.o \
  arch/hal/mouse.o \
  arch/hal/rbaudio.o \
  arch/hal/timer.o \
  arch/hal/window.o

all: descentdroid

include physfs-1.1.1/descentdroid.mk

clean: cleanphysfs
	@echo "CLEAN"
	@rm -f descentdroid $(OBJS)

descentdroid: $(OBJS)
	@echo "LD     $@"
	@$(CC) $(LDFLAGS) -o $@ $^

%.o:%.c
	@echo "CC     $@"
	@$(CC) $(CFLAGS) -c -o $@ $<
