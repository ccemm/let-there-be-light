/*******************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
********************************************************************/
#ifndef DRV_UART_H
#define DRV_UART_H

#include "hw_uart.h"
#include "drvConfig.h"
#include "uart.h"

typedef  void(*UartRcvDataAvlblCallBack)(void);

int drvUARTInit(UartRcvDataAvlblCallBack rcvCallBack);
int drvUARTSend(U8* data, U32 len);
int drvUARTRcv(U8* dest, U32* len);




#endif
