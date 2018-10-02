/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
*******************************************************************************/
#include "drvPshButtons.h"
#include "pigpio.h"

/* Private define ------------------------------------------------------------*/
#define BTN_1_PIN			17
#define BTN_NUM				2

#define SCAN_PERIOD_MS		10					// Set This value as Scan Function Call Period in Ms
#define DEBOUNCE_CNT		100
#define LONG_PRESS_CNT		2000000				// In micro Seconds us
/* Private typedef -----------------------------------------------------------*/
typedef struct _PshBtn
{
	U32 pin;
	U32 cntr;
}PshBtn;
/* Private macro -------------------------------------------------------------*/
static
/* Private variables ---------------------------------------------------------*/
ClickCallback btnPrssEvtCallback;

#define DIFF_TICKS(prev,tnow) ((tnow > prev)? (tnow-prev) : ((0xFFFFFFFF - prev)+tnow+1))


static void keyEvent(int event, int level, uint32_t tick)
{
	static uint32_t prevTick = 0;
	static int prevlevel = 1;

	// Cheated A little bit
	// We will not trigger the event
	// untill user relase the button See Projects .md file
	if(event == BTN_1_PIN && prevlevel == 0 && level == 1 && prevTick != 0)
	{
		uint32_t ticks = DIFF_TICKS(prevTick , tick);
		BtnEvent btnEvent;

		btnEvent.btnNum = 0;
		if(ticks >= LONG_PRESS_CNT)
		{
			btnEvent.Evt = PRESS_LONG;
		}
		else
		{
			btnEvent.Evt = PRESS_SHORT;
		}
		btnPrssEvtCallback(btnEvent);
	}
	prevTick = tick;
	prevlevel = level;
}

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
S32 drvButtonsInit(ClickCallback cBack)
{
	S32 retVal = SUCCESS;

	if(gpioSetMode(17, PI_INPUT)<0 )
	{
		retVal =  FAILURE;
	}

	if(retVal == SUCCESS)
	{
		// Activate Pull-Up for Button
		gpioSetPullUpDown(BTN_1_PIN, PI_PUD_UP); // Sets a pull-down
		// Glitch Filter 100 Ms
		gpioGlitchFilter(BTN_1_PIN, 100000 /*micoseconds == 100Ms */);
		// Set Call Back For Button Event
		gpioSetAlertFunc(BTN_1_PIN, keyEvent);
	}

	btnPrssEvtCallback = cBack;
	return retVal;
}

S32 drvButtonsScan(void)
{
	S32 retVal = SUCCESS;
	// Button Scan Operation Is handled by pgpio library via a daemon process
	return retVal;
}
