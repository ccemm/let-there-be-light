/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
*******************************************************************************/
#include "drvPshButtons.h"
#include "hw_types.h"
#include "hw_gpio.h"
#include "gpio.h"
/* Private define ------------------------------------------------------------*/
#define BTN_1_PIN			GPIO_PIN_4
#define BTN_2_PIN			GPIO_PIN_0
#define BTN_PORTBASE		GPIO_PORTF_BASE
#define BTN_PERIPHERAL		SYSCTL_PERIPH_GPIOF
#define BTN_ALL				(U8)(BTN_1_PIN | BTN_2_PIN)
#define BTN_NUM				2

#define SCAN_PERIOD_MS		1					// Set This value as Scan Function Call Period in Ms
#define DEBOUNCE_CNT		100/SCAN_PERIOD_MS
#define LONG_PRESS_CNT		2000/SCAN_PERIOD_MS
/* Private typedef -----------------------------------------------------------*/
typedef struct _PshBtn
{
	U32 pin;
	U32 cntr;
}PshBtn;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ClickCallback btnPrssEvtCallback = NULL;

static PshBtn pushBtns[BTN_NUM] =
{
		{BTN_1_PIN,0},
		{BTN_2_PIN,0},
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
S32 drvButtonsInit(ClickCallback cBack)
{
	S32 retVal = SUCCESS;

	SysCtlPeripheralEnable(BTN_PERIPHERAL);

    //
    // Unlock PF0 so we can change it to a GPIO input
    // Once we have enabled (unlocked) the commit register then re-lock it
    // to prevent further changes.  PF0 is muxed with NMI thus a special case.
    //
	HWREG(BTN_PORTBASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(BTN_PORTBASE + GPIO_O_CR) |= 0x01;
    HWREG(BTN_PORTBASE + GPIO_O_LOCK) = 0;

	GPIOPinTypeGPIOInput(BTN_PORTBASE, BTN_ALL );
	GPIOPadConfigSet(GPIO_PORTF_BASE, BTN_ALL,
						 GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	btnPrssEvtCallback = cBack;
	return retVal;
}

S32 drvButtonsScan(void)
{
	S32 retVal = SUCCESS;
	U32 i =0;
	BtnEvent btnEvt;
	U32 pinStates =0;

	// Read Pin States
	pinStates = GPIOPinRead(BTN_PORTBASE, BTN_ALL );

	// Check Button States
	for(i =0;i<BTN_NUM;i++)
	{
		// If Btn Pressed
		if( (pinStates & pushBtns[i].pin) == 0 )
		{
			if(pushBtns[i].cntr == LONG_PRESS_CNT)
			{
				// Send Long Pressed Event
				if(btnPrssEvtCallback != NULL)
				{
					btnEvt.btnNum = i;
					btnEvt.Evt = PRESS_LONG;
					btnPrssEvtCallback(btnEvt);
				}
				pushBtns[i].cntr++;
			}
			else if (pushBtns[i].cntr > LONG_PRESS_CNT)
			{
				// Do Nothing Long Pressed Event Already Sent
			}
			else
			{
				// Button Pressed Increment Counter
				pushBtns[i].cntr++;
			}
		}
		else
		{
			if((pushBtns[i].cntr < LONG_PRESS_CNT) && (pushBtns[i].cntr > DEBOUNCE_CNT))
			{
				// Send Short Pressed Event
				if(btnPrssEvtCallback != NULL)
				{
					btnEvt.btnNum = i;
					btnEvt.Evt = PRESS_SHORT;
					btnPrssEvtCallback(btnEvt);
				}
			}
			// Button Release Clear Counter
			pushBtns[i].cntr =0;
		}
	}

	return retVal;
}
