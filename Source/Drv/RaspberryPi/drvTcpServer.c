/*******************************************************************************
* $Author: Cem AKIN
 * $Date: 24.09.2016
 * $Description:
 *
*******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "drvConfig.h"
#include "drvTcpServer.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef  void*(*ThrFunc)(void* params);
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static int servSockFd,clienSockFd;
U8 recvBuf[1024];
OndataRcvCallback rcvCback;
pthread_t thrServHandle;
pthread_t thrClientHandle;

/* Private function prototypes -----------------------------------------------*/
static void* serverTh(void *ptr);
static void* clientRcvTh(void* ptr);
/* Private functions ---------------------------------------------------------*/
static int startThread(ThrFunc thrFunc, pthread_t* thrHndle)
{
	if(pthread_create(thrHndle, NULL, thrFunc, NULL) <0)
	{
		return FAILURE;
	}

	//pthread_join(*thrHndle, NULL);

	return SUCCESS;
}

static void* clientRcvTh(void* ptr)
{
	int rcvLen;
    while ( 1 )
    {
    	//---- wait for a number from client ---
         if ( (rcvLen = read(clienSockFd,recvBuf,sizeof(recvBuf)-1) ) < 0 )
         {
        	 printf( (char*)"ERROR reading from socket");
         }
         recvBuf[rcvLen] = '\0';
         if ( rcvLen <= 0 )
         {
        	 // An Error Occured Or Client Closed The Connection(rcvLen == 0 If client closes the connection!)
        	 break;
         }
        rcvCback(recvBuf,rcvLen);
    }
    close( clienSockFd );
    clienSockFd = 0;
    pthread_exit(ptr);
}

static void* serverTh(void *ptr)
{
	struct sockaddr_in cliAddr;
	int clilen = sizeof(cliAddr);
	int tmpSocket;
    //--- infinite wait on a connection ---
    while ( 1 )
    {
    	if ( ( tmpSocket = accept( servSockFd, (struct sockaddr *) &cliAddr, (socklen_t*) &clilen) ) < 0 )
       {
    	   printf( (char*)"ERROR on accept");
       }
       if(clienSockFd == 0)
       {
    	   clienSockFd = tmpSocket;
    	   startThread(clientRcvTh, &thrClientHandle);
       }
       else
       {
    	   // means there is already a connection ignore incomming request
    	   // ToDo: May be closing current connection and opening a new one
    	   // is a better choice
    	   close(tmpSocket);
       }
    }
}
/* Public functions ---------------------------------------------------------*/

/**
  * @brief:	Starts a TCP server in selected port
  * @param	port: Port Number
  * @param  rcvCallBack: Callback function to call when data received
  * @retval: FAILURE or SUCCESS
  */
int drvTcpServerStartListen(int port, OndataRcvCallback rcvCallBack)
{
	 int retVal = SUCCESS;
     struct sockaddr_in servAddr;

     rcvCback = rcvCallBack;

     servSockFd = socket(AF_INET, SOCK_STREAM, 0);

     if (servSockFd < 0)
     {
    	printf((char*)"ERROR opening socket");
    	retVal = FAILURE;
     }

     if(retVal == SUCCESS)
     {
		 bzero((char *) &servAddr, sizeof(servAddr));

		 servAddr.sin_family = AF_INET;
		 servAddr.sin_addr.s_addr = INADDR_ANY;
		 servAddr.sin_port = htons( port );

		 if (bind(servSockFd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		 {
			 printf((char *)( "ERROR on binding" ) );
			 retVal = FAILURE;
		 }
     }

     if(retVal == SUCCESS)
     {
    	 listen(servSockFd,1);
		// Start Server Thread;
    	 retVal =  startThread(serverTh ,&thrServHandle);
     }

    
	 return retVal;
}

/**
  * @brief:	If there is a connected client in server Sends Data To That Cliena
  * @param  data: Pointer to Data buffer to be send
  * @param  len: length of data
  * @retval: FAILURE or SUCCESS
  */
int drvTcpServerSendToClient(U8* data, int len)
{
	int retVal = SUCCESS;

	if(clienSockFd == 0)
	{
		retVal = FAILURE;
	}

	if(retVal == SUCCESS)
	{
		if (write( clienSockFd, data, len) < 0 )
		{
			retVal = FAILURE;
		}
	}
	return retVal;
}

