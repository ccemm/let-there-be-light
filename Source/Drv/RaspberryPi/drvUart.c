/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
*******************************************************************************/
#include "drvUart.h"
#include "drvRingBuffer.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static UartRcvDataAvlblCallBack dataRcvCallBack = NULL;

/* Private function prototypes -----------------------------------------------*/
static RingBufHndl rcvBufHandle;
static RingBufHndl trnBufHandle;

static U8  rcvRingBuf[UART_BUFFER_SIZE];	// Rcv Buffer
static U8  trnRingBuf[UART_BUFFER_SIZE];	// Trn Buffer

/* Private functions ---------------------------------------------------------*/
static void initUART0(void)
{
	// ToDo:
}

static void uartTrnInt(void)
{
	// ToDo:
}

static void uartRcvInt(void)
{
	//ToDo:
}

/* Public functions ---------------------------------------------------------*/
void UART0IntHandler(void)
{
	// ToDo:
}

/**
  * @brief  Initializes UART0
  * @param  UartRcvDataAvlblCallBack rcvCallBack: Receive Call back Function To Inform App Layer
  * @retval SUCCESS If Operation Sucessfull FAILURE For all other cases
  */
int drvUARTInit(UartRcvDataAvlblCallBack rcvCallBack)
{
	int retVal = SUCCESS;
#if 0
	RingBuffInfo rngBufInfo;

	// Init Rcv Ring Buffer
	rngBufInfo.buffer =  &rcvRingBuf[0];
	rngBufInfo.num_elem = UART_BUFFER_SIZE;
	rngBufInfo.size_elem = sizeof(U8);

	// Init Rcv Ring Buffer
	drvRingBufferInit(&rcvBufHandle,&rngBufInfo );

	// Init Transmit Ring Buffer
	// since other variable are same with rcv buffer, just change actual buffer
	rngBufInfo.buffer =  &trnRingBuf[0];
	drvRingBufferInit(&trnBufHandle,&rngBufInfo );


	dataRcvCallBack = rcvCallBack;

	initUART0();
#endif
	return retVal;
}
/**
  * @brief  Unblocking Send Operation For UART
  * @param  U8* data: pointer to data
  * @param  U32 len: length of data
  * @retval SUCCESS If Operation Sucessfull FAILURE For all other cases
  */
int drvUARTSend(U8* data, U32 len)
{
	int retVal = SUCCESS;
#if 0
	int i =0;

	for(i =0; i<len; i++)
	{
		if(drvRingBufferPut(trnBufHandle, (const void*)&data[i]) != SUCCESS)
		{
			// Buffer Over Flow
			retVal = FAILURE;
			break;
		}
	}

	uartTrnInt();
#endif
	return retVal;
}
/**
  * @brief  Unblocking Receive Operation For UART
  * @param  U8* dest: pointer to read data from receive buffer
  * @param  U32* len: length of received data
  * @retval SUCCESS If Operation Sucessfull FAILURE For all other cases
  */
int drvUARTRcv(U8* dest, U32* len)
{
	int  retVal = SUCCESS;
#if 0
	U8* ptr =dest;

	// ToDo: Add A Get All Function To Ring Buffer
	while(drvRingBufferGet(rcvBufHandle, ptr) == SUCCESS)
	{
		ptr++;
	}

	*len = ptr - dest;

	if(*len == 0)
	{
		retVal = FAILURE;
	}
#endif
	return retVal;
}
