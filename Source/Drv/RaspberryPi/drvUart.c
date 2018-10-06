/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
*******************************************************************************/
#include "drvUart.h"
#include "drvRingBuffer.h"

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static UartRcvDataAvlblCallBack dataRcvCallBack = NULL;
static int uart0Fd = -1;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void initUART0(void)
{
	struct termios options;

	uart0Fd = open( "/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0Fd == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}

	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	tcgetattr(uart0Fd, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0Fd, TCIFLUSH);
	tcsetattr(uart0Fd, TCSANOW, &options);
}

/**
  * @brief  Initializes UART0
  * @param  UartRcvDataAvlblCallBack rcvCallBack: Receive Call back Function To Inform App Layer
  * @retval SUCCESS If Operation Sucessfull FAILURE For all other cases
  */
int drvUARTInit(UartRcvDataAvlblCallBack rcvCallBack)
{
	int retVal = SUCCESS;

	initUART0();
	dataRcvCallBack = rcvCallBack;
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

	if (uart0Fd != -1)
	{
		int count = write(uart0Fd, data, len);		//Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			retVal = FAILURE;
		}
	}
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
	// Read up to 255 characters from the port if they are there
	*len = read(uart0Fd, (void*)dest, 255);

	if(*len <0)
	{
		retVal = FAILURE;
	}

	return retVal;
}

void drvIsRcvDataAvailable(void)
{
	// ToDo: Manage Select structure from a single file for all Events
	//		 Think which one is better with thread or without thread
	//		 For now current implementation is enough for our purpose
	int retVal = SUCCESS;
	fd_set rfds;
	struct timeval tv;

	if(uart0Fd > -1)
	{
		tv.tv_sec = 0;
		tv.tv_usec = 50; /*10000;*/
		// Configure sellect
		/* Watch stdin (fd 0) to see when it has input. */
		FD_ZERO(&rfds);
		FD_SET(uart0Fd, &rfds);
		retVal = select(uart0Fd+1, &rfds, NULL, NULL, &tv);
		if(retVal > 0)
		{
			dataRcvCallBack();
		}
	}
}
