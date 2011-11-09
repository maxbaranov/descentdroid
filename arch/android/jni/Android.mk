# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := native-activity
LOCAL_SRC_FILES := main.c \
  ../../../2d/2dsline.c \
  ../../../2d/bitblt.c \
  ../../../2d/bitmap.c \
  ../../../2d/box.c \
  ../../../2d/canvas.c \
  ../../../2d/circle.c \
  ../../../2d/disc.c \
  ../../../2d/font.c \
  ../../../2d/gpixel.c \
  ../../../2d/line.c \
  ../../../2d/palette.c \
  ../../../2d/pcx.c \
  ../../../2d/pixel.c \
  ../../../2d/poly.c \
  ../../../2d/rect.c \
  ../../../2d/rle.c \
  ../../../2d/scalec.c \
  ../../../3d/clipper.c \
  ../../../3d/draw.c \
  ../../../3d/globvars.c \
  ../../../3d/instance.c \
  ../../../3d/interp.c \
  ../../../3d/matrix.c \
  ../../../3d/points.c \
  ../../../3d/rod.c \
  ../../../3d/setup.c \
  ../../../iff/iff.c \
  ../../../main/ai.c \
  ../../../main/aipath.c \
  ../../../main/automap.c \
  ../../../main/bm.c \
  ../../../main/bmread.c \
  ../../../main/cntrlcen.c \
  ../../../main/collide.c \
  ../../../main/config.c \
  ../../../main/console.c \
  ../../../main/controls.c \
  ../../../main/credits.c \
  ../../../main/custom.c \
  ../../../main/digiobj.c \
  ../../../main/dumpmine.c \
  ../../../main/effects.c \
  ../../../main/endlevel.c \
  ../../../main/fireball.c \
  ../../../main/fuelcen.c \
  ../../../main/fvi.c \
  ../../../main/game.c \
  ../../../main/gamecntl.c \
  ../../../main/gamefont.c \
  ../../../main/gamemine.c \
  ../../../main/gamerend.c \
  ../../../main/gamesave.c \
  ../../../main/gameseg.c \
  ../../../main/gameseq.c \
  ../../../main/gauges.c \
  ../../../main/hash.c \
  ../../../main/hostage.c \
  ../../../main/hud.c \
  ../../../main/inferno.c \
  ../../../main/kconfig.c \
  ../../../main/kmatrix.c \
  ../../../main/laser.c \
  ../../../main/lighting.c \
  ../../../main/menu.c \
  ../../../main/mglobal.c \
  ../../../main/mission.c \
  ../../../main/morph.c \
  ../../../main/newdemo.c \
  ../../../main/newmenu.c \
  ../../../main/object.c \
  ../../../main/paging.c \
  ../../../main/physics.c \
  ../../../main/piggy.c \
  ../../../main/player.c \
  ../../../main/playsave.c \
  ../../../main/polyobj.c \
  ../../../main/powerup.c \
  ../../../main/render.c \
  ../../../main/robot.c \
  ../../../main/scores.c \
  ../../../main/slew.c \
  ../../../main/snddecom.c \
  ../../../main/songs.c \
  ../../../main/state.c \
  ../../../main/switch.c \
  ../../../main/terrain.c \
  ../../../main/texmerge.c \
  ../../../main/text.c \
  ../../../main/titles.c \
  ../../../main/vclip.c \
  ../../../main/wall.c \
  ../../../main/weapon.c \
  ../../../main/multi.c \
  ../../../maths/fixc.c \
  ../../../maths/rand.c \
  ../../../maths/tables.c \
  ../../../maths/vecmat.c \
  ../../../mem/mem.c \
  ../../../misc/args.c \
  ../../../misc/dl_list.c \
  ../../../misc/error.c \
  ../../../misc/hmp.c \
  ../../../misc/ignorecase.c \
  ../../../misc/physfsx.c \
  ../../../misc/strio.c \
  ../../../misc/strutil.c \
  ../../../texmap/ntmap.c \
  ../../../texmap/scanline.c \
  ../../../main/multibot.c \
  ../../../arch/hal/digi_null.c \
  ../../../arch/hal/digi.c \
  ../../../arch/hal/event.c \
  ../../../arch/hal/gr.c \
  ../../../arch/hal/ogl.c \
  ../../../arch/hal/init.c \
  ../../../arch/hal/joy.c \
  ../../../arch/hal/key.c \
  ../../../arch/hal/mouse.c \
  ../../../arch/hal/rbaudio.c \
  ../../../arch/hal/timer.c \
  ../../../arch/hal/window.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../ $(LOCAL_PATH)/../../../include/ $(LOCAL_PATH)/../../../main/ $(LOCAL_PATH)/../../../arch/include/
LOCAL_CFLAGS := -DNETWORK -DOGL -DOPENGLES
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
