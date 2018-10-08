/*******************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
********************************************************************/
#ifndef DRV_UART_H
#define DRV_UART_H

#if PART_TM4C123GH6PM
	#include "hw_uart.h"
	#include "drvConfig.h"
	#include "uart.h"
#endif
#include "drvConfig.h"



typedef  void(*UartRcvDataAvlblCallBack)(void);

int drvUARTInit(UartRcvDataAvlblCallBack rcvCallBack);
int drvUARTSend(U8* data, U32 len);
int drvUARTRcv(U8* dest, U32* len);
void drvUARTIsRcvAvail(void);


#endif
