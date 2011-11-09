#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include "osal.h"

typedef struct {
  FILE *file;
} OSALFile;

OSALFile *OSAL_fopen(const char *pzFile,const char *pzMode) {
  FILE *f;
  OSALFile *pRet;

  f = fopen(pzFile,pzMode);
  if( f == NULL )
    return NULL;

  pRet = (OSALFile*)calloc(1,sizeof(OSALFile));
  pRet->file = f;

  return pRet;
}

void      OSAL_fclose(OSALFile *pFile) {
  if( pFile == NULL )
    return;

  fclose(pFile->file);
  free(pFile);
}

int       OSAL_read(OSALFile *pFile,void *buf,int size,int count) {
  return read(pFile->file,buf,size*count);
}

int       OSAL_tell(OSALFile *pFile) {
  return ftell(pFile->file);
}

int       OSAL_readInt(OSALFile *pFile) {
  int r;
  
  read(pFile->file,&r,sizeof(int));
  return r;
}

signed short OSAL_readShort(OSALFile *pFile) {
  signed short r;
  
  read(pFile->file,&r,sizeof(signed short));
  return r;
}
