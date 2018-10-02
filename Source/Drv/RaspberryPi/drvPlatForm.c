/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date: Kick Start 24.09.2016
 * $Description:
 *
*******************************************************************************/
#include "drvPlatform.h"
#include "pigpio.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes Tiva Platform
  * @param  None
  * @retval None
  */
S32 drvInitPlatform(void)
{
	S32 retVal = SUCCESS;
	if(gpioInitialise()<0)
	{
		retVal = FAILURE;
	}
	return retVal;
}
