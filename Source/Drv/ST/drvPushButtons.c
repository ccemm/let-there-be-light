#if 0

/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
*******************************************************************************/
#include "drvPshButtons.h"
#include "gpio.h"
#include "stm32l100xc.h"
/* Private define ------------------------------------------------------------*/
#define BTN_1_PIN			4
#define BTN_2_PIN			0  // ! This Pin Does not exist in STL100RC KIT
#define BTN_PORTBASE		GPIOA
#define BTN_PERIPHERAL		SYSCTL_PERIPH_GPIOF
#define BTN_ALL				(U8)(BTN_1_PIN | BTN_2_PIN)
#define BTN_NUM				1  // ! One Button On STL100RC Kit

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
		{BTN_1_PIN,0}
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
S32 drvButtonsInit(ClickCallback cBack)
{
	S32 retVal = SUCCESS;
	// Enable the GPIO-C port that is used for the on-board LED.
    RCC->AHBENR|=0x00008001;

    // Configure Button Pin As Input
    GPIOC->MODER=0x55555555;

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
    // !!!Warning= If Buttons Conected to different Ports Correct it in here
	pinStates = BTN_PORTBASE->IDR;

	// Check Button States
    
    // !!!Warning: This For Loop may cause trouble if Buttons connected to different pins 
	for(i =0;i<BTN_NUM;i++)
	{
		// If Btn Pressed
		if( (pinStates & (1<<pushBtns[i].pin)) == 0 )
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


#endif

