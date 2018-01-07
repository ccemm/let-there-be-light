/********************************************************************
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
********************************************************************/
#include "drvRingBuffer.h"
#include "string.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct _RingBuffer
{
	U32 size_elem;
	U32 num_elem;
    U8 *buf;
    volatile U32 head;
    volatile U32 tail;
}RingBuffer;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static RingBuffer ringBuffers[RING_BUFFER_MAX];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int isRngBuffFull(RingBuffer *rb)
{
    return ((rb->head - rb->tail) == rb->num_elem) ? 1 : 0;
}

static int isRngBuffEmpty(RingBuffer *rb)
{
    return ((rb->head - rb->tail) == 0U) ? 1 : 0;
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief To Init Ring Buffer
  * @param
  * @retval:  SUCCESS or FAILURE
  * @warning: User Must Provide Actual buffer In memory and should always
  *           Access That Buffer Via This Interface afterwards
  */
S32 drvRingBufferInit(RingBufHndl* hanlde, RingBuffInfo *ringBufInfo)
{
    static int idx = 0;
    int retVal = FAILURE;

    if ((idx < RING_BUFFER_MAX) && (hanlde != NULL) && (ringBufInfo != NULL))
    {
        if ((ringBufInfo->buffer != NULL) && (ringBufInfo->size_elem > 0))
        {
            // Ring buffer Size should be a Power Of 2, This will make things easier for Us
            if (((ringBufInfo->num_elem - 1) & ringBufInfo->num_elem) == 0)
            {
                // Initialize the ring buffer internal variables
            	ringBuffers[idx].head = 0;
            	ringBuffers[idx].tail = 0;
            	ringBuffers[idx].buf = ringBufInfo->buffer;
            	ringBuffers[idx].size_elem = ringBufInfo->size_elem;
            	ringBuffers[idx].num_elem = ringBufInfo->num_elem;

                *hanlde = idx++;
                retVal= SUCCESS;
            }
        }
    }

    return retVal;
}
/**
  * @brief  To Write data to Ring Buffer
  * @param
  * @retval  SUCCESS or FAILURE
  */
S32 drvRingBufferPut(RingBufHndl hndl, const void *data)
{
    int retVal = SUCCESS;

    if ((hndl < RING_BUFFER_MAX) && (isRngBuffFull(&ringBuffers[hndl]) == 0))
    {
    	// head & (num_elem -1) = element in ring buffer
    	// For Example If Head Is At 100 hundered and buffer is 8 bytes long
    	// The new value should be inserted to 4th index (100 mod 8)
    	// Instead of modulus we use & operation to calculate Required Index.
    	// Note: For doing that buffer size must be a power of two
    	// 8-1=7=0b0111
    	// 100=0b0100\_0100
    	// 0b0111\ \&\ 0b0100\_0100=0b100=4
        // Since our element size is variable To find actual index
    	// We also need to multiply result with actual element sşze
    	U32 offset = (ringBuffers[hndl].head & (ringBuffers[hndl].num_elem - 1)) * ringBuffers[hndl].size_elem;

        memcpy(&(ringBuffers[hndl].buf[offset]), data, ringBuffers[hndl].size_elem);
    	//ringBuffers[rbd].buf[offset] =  data;
        ringBuffers[hndl].head++;
    }
    else
    {
    	retVal = FAILURE;
    }

    return retVal;
}

/**
  * @brief  To Read Data From Ring Buffer
  * @param
  * @retval  SUCCESS or FAILURE
  */
S32 drvRingBufferGet(RingBufHndl hndl, void *data)
{
    int retVal = SUCCESS;

    if ((hndl < RING_BUFFER_MAX) && (isRngBuffEmpty(&ringBuffers[hndl]) == 0))
    {
        U32 offset = (ringBuffers[hndl].tail & (ringBuffers[hndl].num_elem - 1)) * ringBuffers[hndl].size_elem;

        memcpy(data, &(ringBuffers[hndl].buf[offset]), ringBuffers[hndl].size_elem);
        ringBuffers[hndl].tail++;
    }
    else
    {
    	retVal = FAILURE;
    }

    return retVal;
}
/**
  * @brief  To Get Current Element Number In Ring Buffer
  * @param
  * @retval  Element Number In Ring Buffer
  */
U32 drvRingBufferElemNum(RingBufHndl hndl)
{
	return (ringBuffers[hndl].head - ringBuffers[hndl].tail);
}

