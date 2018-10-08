/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
*******************************************************************************/
#include "drvUart.h"
#include "drvRingBuffer.h"

#include "hw_types.h"
#include "gpio.h"
#include "interrupt.h"
#include "pin_map.h"
#include "sysctl.h"


#include "uart.h"
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
	// Enable the peripherals used by this example.
	// The UART itself needs to be enabled, as well as the GPIO port
	// containing the pins that will be used.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable processor interrupts.
    IntMasterEnable();

	// Configure the GPIO pin muxing for the UART function.
	// This is only necessary if your part supports GPIO pin function muxing.
	// Study the data sheet to see which functions are allocated per pin.
	// TODO: change this to select the port/pin you are using
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

	// Since GPIO A0 and A1 are used for the UART function, they must be
	// configured for use as a peripheral function (instead of GPIO).
	// TODO: change this to match the port/pin you are using
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// Configure the UART for 115,200, 8-N-1 operation.
	// This function uses SysCtlClockGet() to get the system clock
	// frequency.  This could be also be a variable or hard coded value
	// instead of a function call.
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
							(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	                         UART_CONFIG_PAR_NONE));

	// Enable UART Fifo
	UARTFIFOEnable(UART0_BASE);
	UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8,UART_FIFO_RX7_8);

	// Enable the UART interrupt.
	UARTTxIntModeSet(INT_UART0,UART_TXINT_MODE_FIFO);
	IntEnable(INT_UART0);
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	UARTEnable(UART0_BASE);
}

static void uartTrnInt(void)
{
	U8  trnByte =0;

	UARTIntDisable(UART0_BASE, UART_INT_TX);
	while(1)
	{
		if( UARTSpaceAvail(UART0_BASE))
		{
			if(drvRingBufferGet(trnBufHandle, &trnByte) != SUCCESS)
			{
				// Means No More Data At the buffer
				trnByte =0;
				break;
			}
			// Transmit Fifo Buffer Is Full Put Last Byte Back
			UARTCharPutNonBlocking(UART0_BASE,trnByte);
		}
		else
		{
			break;
		}
	}

	if(drvRingBufferElemNum(trnBufHandle) > 0)
	{
		UARTIntEnable(UART0_BASE, UART_INT_TX);
	}
}

static void uartRcvInt(void)
{
	S32 ret =0;
	U8	rcvByte =0;
	// Loop while there are characters in the receive FIFO.
	while(UARTCharsAvail(UART0_BASE))
	{
		// Read the next character from the UART and Write It To Ring Buffer
		ret = UARTCharGetNonBlocking(UART0_BASE);
		if(ret != -1)
		{
			rcvByte = (U8)ret;
			if (drvRingBufferPut(rcvBufHandle,&rcvByte) != SUCCESS)
			{
				// ToDO: Means Ring Buffer Is Full Think What To Do.
			}
		}
		else
		{
			// ToDO: Means There is No Data On Rcv Buffer Think What To Do.
		}
	}
    // callback to inform user
	if(dataRcvCallBack != NULL)
	{
		dataRcvCallBack();
	}
}

/* Public functions ---------------------------------------------------------*/
void UART0IntHandler(void)
{
	U32 ui32Status;
	U32 int_type;

	// Get the interrrupt status.
    ui32Status = UARTIntStatus(UART0_BASE, true);
    int_type = (uint8_t)((ui32Status & 0x0F0) >> 4);

    // Clear the asserted interrupts.
    UARTIntClear(UART0_BASE, 0xFFFFFFFF);

    if(int_type == 2 ) //UART_INT_TX
    {
    	uartTrnInt();
    }
    else if((int_type == 1) || (int_type == 4)) //UART_INT_RX
    {
    	uartRcvInt();
    }
}

/**
  * @brief  Initializes UART0
  * @param  UartRcvDataAvlblCallBack rcvCallBack: Receive Call back Function To Inform App Layer
  * @retval SUCCESS If Operation Sucessfull FAILURE For all other cases
  */
int drvUARTInit(UartRcvDataAvlblCallBack rcvCallBack)
{
	int retVal = SUCCESS;
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

	return retVal;
}

void drvUARTIsRcvAvail(void)
{
	return FAILURE;
}
