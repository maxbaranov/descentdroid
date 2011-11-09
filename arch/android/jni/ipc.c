#include <string.h>
#include <pthread.h>

#include "ipc.h"

static pthread_mutex_t queuemutex = PTHREAD_MUTEX_INITIALIZER;

#define QUEUE_SIZE 32
static struct {
  IPCEvent_t ev;
} queue[QUEUE_SIZE];

int IPCEvent_Post(IPCEvent_t *pEv) {
  int i;

  pthread_mutex_lock( &queuemutex );

  for(i=0;i<QUEUE_SIZE;i++) {
    if( queue[i].ev.type == eEventType_Invalid ) {
      memcpy(&queue[i].ev,pEv,sizeof(IPCEvent_t));
      pthread_mutex_unlock( &queuemutex );
      return 0;
    }
  }

  pthread_mutex_unlock( &queuemutex );

  return -1;
}

int IPCEvent_Poll(IPCEvent_t *pEv) {
  int i;

  pthread_mutex_lock( &queuemutex );

  for(i=0;i<QUEUE_SIZE;i++) {
    if( queue[i].ev.type != eEventType_Invalid ) {
      memcpy(pEv,&queue[i].ev,sizeof(IPCEvent_t));
      queue[i].ev.type = eEventType_Invalid;
      pthread_mutex_unlock( &queuemutex );
      return 0;
    }
  }

  pthread_mutex_unlock( &queuemutex );

  return -1;
}
