/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
*******************************************************************************/
#include "drvConfig.h"
#include "drvDebug.h"
//#include "drvUart.h"
#include "stdarg.h"
#include "string.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

int drvDebugInit(void)
{
	return SUCCESS;
}

int drvSetInterface(int iface)
{
	return SUCCESS;
}

void drvPrintf(const char * format, ...)
{
	U8 tmpStr[512] = {0};
	va_list args;

	va_start(args,format);
	vsprintf((char*)tmpStr, format , args);
	va_end(args);

	printf("%s \n", tmpStr);
}

