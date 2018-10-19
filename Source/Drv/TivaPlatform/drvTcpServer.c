/*******************************************************************************
* $Author: Cem AKIN
 * $Date: 24.09.2016
 * $Description:
 *
*******************************************************************************/
#include "drvConfig.h"
#include "drvTcpServer.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//static void sendToClient(void);
/* Private functions ---------------------------------------------------------*/
/* Public functions ---------------------------------------------------------*/

/**
  * @brief:	Starts a TCP server in selected port
  * @param	port: Port Number
  * @param  rcvCallBack: Callback function to call when data received
  * @retval: FAILURE or SUCCESS
  */
int drvTcpServerStartListen(int port, OndataRcvCallback rcvCallBack)
{
	 int retVal = FAILURE;
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
    int retVal = FAILURE;
	return retVal;
}

