/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
*******************************************************************************/
#include "drvConfig.h"

#include "pigpio.h"

#include "drvLeds.h"
/* Private define ------------------------------------------------------------*/
#define LED_PIN		18

#define LED_TICK_PERIOD_MS		10
#define LED_TICK_CNT_1SEC		(int)(1000/LED_TICK_PERIOD_MS)
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static BOOL ledIsOn = FALSE;
/* Private function prototypes -----------------------------------------------*/
static LedBlinkMode gBlinkMode = LED_BLNK_NONE;

static const int periodChangeSec[LED_BLNK_NUM] = {0,4,8};	// 2*(1 sec On 1 sec Off) Change Mode after 4 sec,
															// 2*(2 sec On 2 sec) Change Mode aftr 8 sec
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Acording To Requirement Document Of test Projects Leds Should Be able to blink In One of Two
  *			Patterns 2*(1sec On 1sec Off) Contiuing with 2*(2sec On 2sec Off)
  *			And  	 4*(1sec On 1sec Off) Contiuing with 4*(2sec On 2sec Off).
  *			If you call this function with a 1 sec period and Set "gBlinkMode" as Requiered Led Blink Pattern
  *			This function handles rest
  * @param  None
  * @retval SUCCESS or FAILURE
  */
static void heartBeatLed(void)
{
	static int cntr1 =0;

	if( (cntr1 <= periodChangeSec[gBlinkMode]) || ((cntr1%2) == 0))
	{
		drvLedToggle();
	}
	cntr1++;
	if(cntr1 >= (3*periodChangeSec[gBlinkMode]))
	{
		cntr1 =0;
	}
}
/* Public functions ---------------------------------------------------------*/
S32 drvLedsInit(void)
{
	S32 retVal = SUCCESS;
	if(gpioSetMode(LED_PIN, PI_INPUT)<0)
	{
		retVal = FAILURE;
	}

	return retVal;
}
/**
  * @brief:	Led State set to On If blink mode is not idle led change state on next cycle.
  * @param  None
  * @retval SUCCESS If Operation Sucessfull FAILURE For all other cases
  */
void drvLedOn(void)
{
	gpioWrite(LED_PIN,1);
	ledIsOn = TRUE;
}
/**
  * @brief:	Led State set to Off If blink mode is not idle led change state on next cycle.
  * @param  None
  * @retval SUCCESS If Operation Sucessfull FAILURE For all other cases
  */
void drvLedOff(void)
{
	gpioWrite(LED_PIN,0);
	ledIsOn = FALSE;
}

void drvLedToggle(void)
{
	if(ledIsOn == TRUE)
	{
		drvLedOff();
	}
	else
	{
		drvLedOn();
	}
}

int drvLedSetBlinkMode(LedBlinkMode blnkMode)
{
	int retVal = SUCCESS;
	if(blnkMode< LED_BLNK_NUM)
	{
		gBlinkMode = blnkMode;
	}
	else
	{
		retVal = FAILURE;
	}
	return retVal;
}

/**
  * @brief:	Handle Led Blinks according to choosen blink mode If Called with 1ms period
  * @param  None
  * @retval None
  */
void drvLedOnTimeTick(void)
{
	static int cntrTimePeriod =0;
	if( gBlinkMode != LED_BLNK_NONE)
	{
		cntrTimePeriod++;
		if(cntrTimePeriod == LED_TICK_CNT_1SEC)
		{
			cntrTimePeriod =0;
			heartBeatLed();
		}
	}
}
