/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
*******************************************************************************/
#include "drvPshButtons.h"
#include "drvLeds.h"
#include "middStateMngr.h"
#include "appStates.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static SttRet btnPressed(BtnEvent btnEvt);
State sttIde;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static SttRet btnPressed(BtnEvent btnEvt)
{
	//if(btnEvt.btnNum == 1)
	{
		if(btnEvt.Evt == PRESS_LONG)
		{
			drvLedSetBlinkMode(LED_BLNK_41_42);
		}
		else
		{
			drvLedSetBlinkMode(LED_BLNK_21_22);
		}
	}
	return STT_RET_STAY;
}
/* Public functions ---------------------------------------------------------*/
int appStateInit(void)
{
	sttIde.onTimeTick = NULL;
	sttIde.btn_pressed =btnPressed;
	sttIde.nextState = NULL;
	sttIde.prevtState = NULL;
	middStateInit(&sttIde);
	return SUCCESS;
}
