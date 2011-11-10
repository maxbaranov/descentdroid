#pragma once

typedef enum {
  eEventType_Invalid = 0,
  eEventType_Key     = 0x10,
} eEventType;

typedef enum {
  eEventKey_None = 0x00,
  eEventKey_L1,
  eEventKey_R1,
  eEventKey_START,
  eEventKey_SELECT,
  eEventKey_DPAD_UP,
  eEventKey_DPAD_DOWN,
  eEventKey_DPAD_LEFT,
  eEventKey_DPAD_RIGHT,
  eEventKey_CROSS,
  eEventKey_SQUARE,
  eEventKey_TRIANGLE,
  eEventKey_CIRCLE,
  eEventKey_Menu,
} eEventKey;

typedef struct {
  eEventType type;

  union {
    struct {
      int       state;
      eEventKey key;
    } key;
  };
} IPCEvent_t;

int IPCEvent_Post(IPCEvent_t *pEv);
int IPCEvent_Poll(IPCEvent_t *pEv);

void IPCQuitApp(void);
int  IPCWantsToQuit(void);
