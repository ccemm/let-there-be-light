/********************************************************************
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
********************************************************************/
#ifndef DRV_RING_BUFFER_H
#define DRV_RING_BUFFER_H

#include "drvConfig.h"



typedef unsigned int RingBufHndl;

typedef struct _RingBuffInfo
{
    U32 size_elem;
    U32 num_elem;
    void *buffer;
} RingBuffInfo;

S32 drvRingBufferInit(RingBufHndl* hanlde, RingBuffInfo *ringBufInfo);
S32 drvRingBufferPut(RingBufHndl rbd, const void *data);
S32 drvRingBufferGet(RingBufHndl rbd, void *data);
U32 drvRingBufferElemNum(RingBufHndl rbd);
#endif
