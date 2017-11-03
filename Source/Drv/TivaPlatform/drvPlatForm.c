/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date: Kick Start 24.09.2016
 * $Description:
 *
*******************************************************************************/
#include "drvPlatform.h"

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
	// Configure Clock
	// Set the clocking to run directly from the crystal.
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
			SYSCTL_XTAL_16MHZ);
	return retVal;
}
