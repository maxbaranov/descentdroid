//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <android/log.h>
#include <pthread.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "ipc.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "descent", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "descent", __VA_ARGS__))

/**
 * Shared state for our app.
 */
struct engine 
{
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
};



/* Containts the path to /data/data/(package_name)/libs */
static char* lib_dir=NULL;

static jboolean audioEnabled=1;
static jboolean benchmarkEnabled=0;
static jboolean lightmapsEnabled=0;
static jboolean showFramerateEnabled=0;
static jobject audioBuffer=0;
static jobject kwaakAudioObj=0;
static void *libdl;
static int init=0;

typedef unsigned char BOOL;
#define FALSE 0
#define TRUE 1

int GetPos()
{
  return 0;
}

void InitAudio(void *buffer, int size)
{
}

void WriteAudio(int offset, int length)
{
}
#define DEBUG 1

void OnLoad()
{
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Descent_JNI", "JNI_OnLoad called");
#endif
}

int  gameloop_init(int argc, char *argv[]);
int  gameloop_tick(void);
void gameloop_cleanup(void);

void ogl_set_sd(EGLSurface s,EGLDisplay d);

void destroyGLESContext(struct engine *engine) {
  if( engine->display != EGL_NO_DISPLAY ) {
    eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if( engine->context != EGL_NO_CONTEXT ) {
      eglDestroyContext(engine->display, engine->context);
    }
    if( engine->surface != EGL_NO_SURFACE ) {
      eglDestroySurface(engine->display, engine->surface);
    }
    eglTerminate(engine->display);
  }
  engine->animating = 0;
  engine->display = EGL_NO_DISPLAY;
  engine->context = EGL_NO_CONTEXT;
  engine->surface = EGL_NO_SURFACE;
}

int createGLESContext(struct engine *engine) {
    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_BLUE_SIZE, 5,
      EGL_GREEN_SIZE, 6,
      EGL_RED_SIZE, 5,
      EGL_DEPTH_SIZE, 8,
      EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->animating = 1;

    ogl_set_sd(surface,display);

    return 0;
}

void *gamethread_fn(void *arg) {
  struct engine* engine = (struct engine*)arg;
  char *argv[] = { "descent","-hogdir","/sdcard/Descent/", NULL };

  if( createGLESContext(engine) != 0 )
    return NULL;

  gameloop_init(3,argv);

  while(gameloop_tick() == 0) { }

  gameloop_cleanup();

  destroyGLESContext(engine);
  IPCQuitApp();

  return NULL;
}

void InitGame(struct engine* engine)
{
  char *argv[] = { "descent","-hogdir","/sdcard/Descent/",NULL };
  int argc = 3;

    if(!init) {
      pthread_t gameThread;
      pthread_create(&gameThread,NULL,gamethread_fn,engine);

      init = 1;
    }

#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Descent_JNI", "initGame(%p)", engine);
#endif

    //setAudioCallbacks(&GetPos, &WriteAudio, &InitAudio);
    //setResolution(width, height);
}

void QueueMotionEvent(jint action, jfloat x, jfloat y, jfloat pressure)
{
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Descent_JNI", "queueMotionEvent(%d, %f, %f, %f)", action, x, y, pressure);
#endif
    //if(queueMotionEvent) queueMotionEvent(action, x, y, pressure);
}

void QueueTrackballEvent(jint action, jfloat x, jfloat y)
{
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Descent_JNI", "queueTrackballEvent(%d, %f, %f)", action, x, y);
#endif
    //if(queueTrackballEvent) queueTrackballEvent(action, x, y);
}

void QueueJoystickEvent(jint nAxis, jint nValue)
{
#ifdef DEBUG
    __android_log_print(ANDROID_LOG_DEBUG, "Descent_JNI", "queueJoystickEvent(%d, %d)", nAxis, nValue);
#endif
    //if(queueJoystickEvent) queueJoystickEvent(nAxis, nValue);
}

static int androidKeyCodeToDescent(AInputEvent* aevent,int state)
{	
  int32_t a_keycode = (AKeyEvent_getKeyCode)(aevent);
  IPCEvent_t ev;
  int bHasEvent = 1;

  ev.type = eEventType_Key;
  ev.key.state = !!state;

  switch(a_keycode) {
  case AKEYCODE_BUTTON_SELECT:
    ev.key.key = eEventKey_SELECT;
    break;
  case AKEYCODE_BUTTON_START:
    ev.key.key = eEventKey_START;
    break;
  case AKEYCODE_DPAD_UP:
    ev.key.key = eEventKey_DPAD_UP;
    break;
  case AKEYCODE_DPAD_DOWN:
    ev.key.key = eEventKey_DPAD_DOWN;
    break;
  case AKEYCODE_DPAD_LEFT:
    ev.key.key = eEventKey_DPAD_LEFT;
    break;
  case AKEYCODE_DPAD_RIGHT:
    ev.key.key = eEventKey_DPAD_RIGHT;
    break;
  case AKEYCODE_BUTTON_L1:
    ev.key.key = eEventKey_L1;
    break;
  case AKEYCODE_BUTTON_R1:
    ev.key.key = eEventKey_R1;
    break;
  case AKEYCODE_DPAD_CENTER:
    ev.key.key = eEventKey_CROSS;
    break;
  case AKEYCODE_BUTTON_X:
    ev.key.key = eEventKey_SQUARE;
    break;
  case AKEYCODE_BUTTON_Y:
    ev.key.key = eEventKey_TRIANGLE;
    break;
  case AKEYCODE_BACK:
    ev.key.key = eEventKey_CIRCLE;
    break;
  default:
    __android_log_print(ANDROID_LOG_DEBUG, "Descent_JNI", "androidKeyCodeToDescent(%i)",a_keycode);
    bHasEvent = 0;
  }

  if( bHasEvent ) {
    IPCEvent_Post(&ev);
  //__android_log_print(ANDROID_LOG_DEBUG, "Descent_JNI", "androidKeyCodeToDescent(%i,%d,%d)",a_keycode,ev.key.key,ev.key.state);
    return 0;
  }

  return 1;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
  struct engine* engine = (struct engine*)app->userData;
  switch (AInputEvent_getType(event)) {
  case AINPUT_EVENT_TYPE_KEY:
    {
      int32_t nKeyAction = (AKeyEvent_getAction)(event);
      int32_t a_keycode = (AKeyEvent_getKeyCode)(event);
      int32_t a_scancode = (AKeyEvent_getScanCode)(event);
      int32_t a_metastate = (AKeyEvent_getMetaState)(event);
      int bAltPressed = (a_metastate & AMETA_ALT_ON)!=0;
      int nValue = 0;
      if( nKeyAction == AKEY_EVENT_ACTION_DOWN )
	nValue = 1;
      int nDescentKey = androidKeyCodeToDescent(event,nValue);
      return !nDescentKey;
    }
    break;
		
  case AINPUT_EVENT_TYPE_MOTION:
    {
      int32_t nSourceId = (AInputEvent_getSource)(event);
      if(nSourceId == AINPUT_SOURCE_TOUCHPAD) {
	//QueueJoystickEvent(0, AMotionEvent_getX(event,0));
	float fX = AMotionEvent_getX(event, 0);
	float fY = AMotionEvent_getY(event, 0);
	fX = fX/966.0f*854.0f;
	fY = 480.0f-fY/360.0f*480.0f;
	QueueMotionEvent((AKeyEvent_getAction)(event), fX, fY, 1);
      } else {
	//engine->animating = 1;
	//engine->state.x = AMotionEvent_getX(event, 0);
	//engine->state.y = AMotionEvent_getY(event, 0);
	QueueMotionEvent((AKeyEvent_getAction)(event), AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0), 1);
      }
    }
    return 1;
  }
  return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) 
{
  static int bInit = 0;
  struct engine* engine = (struct engine*)app->userData;
  switch (cmd) {
  case APP_CMD_SAVE_STATE:
    // The system has asked us to save our current state.  Do so.
    //engine->app->savedState = malloc(sizeof(struct saved_state));
    //*((struct saved_state*)engine->app->savedState) = engine->state;
    //engine->app->savedStateSize = sizeof(struct saved_state);
    break;
  case APP_CMD_INIT_WINDOW:
    // The window is being shown, get it ready.
    if (engine->app->window != NULL) {
      //engine_init_display(engine);
      if (!bInit) {
	InitGame(engine);
	bInit = 1;
      }
      //engine_draw_frame(engine);
    }
    break;
  case APP_CMD_TERM_WINDOW:
    // The window is being hidden or closed, clean it up.
    //engine_term_display(engine);
    exit(0);
    break;
  case APP_CMD_GAINED_FOCUS:
    // When our app gains focus, we start monitoring the accelerometer.
    if(engine->accelerometerSensor != NULL) {
#if 0
      ASensorEventQueue_enableSensor(engine->sensorEventQueue,
				     engine->accelerometerSensor);
      // We'd like to get 60 events per second (in us).
      ASensorEventQueue_setEventRate(engine->sensorEventQueue,
				     engine->accelerometerSensor, (1000L/60)*1000);
#endif
      engine->animating = 1;
    }
    break;
  case APP_CMD_LOST_FOCUS:
    // When our app loses focus, we stop monitoring the accelerometer.
    // This is to avoid consuming battery while not being used.
    if(engine->accelerometerSensor != NULL) {
      ASensorEventQueue_disableSensor(engine->sensorEventQueue,
				      engine->accelerometerSensor);
    }
    // Also stop animating.
    engine->animating = 0;
    //engine_draw_frame(engine);
    break;
  }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
  struct engine engine;

  // Make sure glue isn't stripped.
  app_dummy();

  // Load library
  OnLoad();
	
  memset(&engine, 0, sizeof(engine));
  state->userData = &engine;
  state->onAppCmd = engine_handle_cmd;
  state->onInputEvent = engine_handle_input;
  engine.app = state;

  // Prepare to monitor accelerometer
  engine.sensorManager = ASensorManager_getInstance();
  engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
							       ASENSOR_TYPE_ACCELEROMETER);
  engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
							    state->looper, LOOPER_ID_USER, NULL, NULL);
  
  if(state->savedState != NULL) {
    // We are starting with a previous saved state; restore from it.
    //engine.state = *(struct saved_state*)state->savedState;
  }

  // loop waiting for stuff to do.

  while(1) {
    // Read all pending events.
    int ident;
    int events;
    struct android_poll_source* source;

    // If not animating, we will block forever waiting for events.
    // If animating, we loop until all events are read, then continue
    // to draw the next frame of animation.
    while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
				  (void**)&source)) >= 0) {
      // Process this event.
      if (source != NULL) {
	source->process(state, source);
      }

      // If a sensor has data, process it now.
      if (ident == LOOPER_ID_USER) {
	if (engine.accelerometerSensor != NULL) {
	  ASensorEvent event;
	  while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
					     &event, 1) > 0) {
	    /*LOGI("accelerometer: x=%f y=%f z=%f",
	      event.acceleration.x, event.acceleration.y,
	      event.acceleration.z);
	    */
	  }
	}
      }

      // Check if we are exiting.
      if( IPCWantsToQuit() )
	exit(0);
    }
  }
}
//END_INCLUDE(all)
