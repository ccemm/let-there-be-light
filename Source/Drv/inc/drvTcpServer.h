/*******************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
********************************************************************/
#ifndef _DRV_TCPSERVER_H
#define _DRV_TCPSERVER_H

#include "genericTypes.h"

typedef void (*OndataRcvCallback)(unsigned char* data, int len);

int drvTcpServerStartListen(int port, OndataRcvCallback rcvCallBack);
int drvTcpServerSendToClient(U8* data, int len);

#endif
