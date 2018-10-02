/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date: Kick Start 24.09.2016
 * $Description:
 *
*******************************************************************************/
#include "genericTypes.h"
#include "drvConfig.h"
#include "drvPlatform.h"
#include "drvTimers.h"
#include "drvLeds.h"
#include "drvPshButtons.h"
#include "drvUart.h"
#include "drvDebug.h"

#if PART_TM4C123GH6PM
#include "hw_types.h"
#include "hw_gpio.h"
#include "gpio.h"

#endif

#include "middStateMngr.h"
#include "appStates.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct _Loop
{
	void (*timeTickMs)(void);
	void (*uart0OnDataRcv)(void);
	void (*uart1OnDataRcv)(void);
	void (*onButtonEvent)(BtnEvent btnEvt);
}Loop;

typedef enum _EventsSys
{
	EVT_TIME_TICK = 0,
	EVT_UART_0 = 1,
	EVT_UART_1 = 2,
	EVT_BTN = 3,
	EVT_END
}EventsSys;

/* Private macro -------------------------------------------------------------*/
#define IS_EVT_OCCUR(reg,evtnum)	((reg & (1<<evtnum)))
#define SET_EVT_FLAG(reg,evtnum)	(reg |= (1<<evtnum));
#define RESET_EVT_FLAG(reg,evtnum)	(reg &= ~(1<<evtnum));

/* Private function prototypes -----------------------------------------------*/
static void loopTimeTick(void);
static void loopOnBtnEvt(BtnEvent bntEvt);
/* Private variables ---------------------------------------------------------*/
static TimerNum timer1Ms = INV_TIMER_NUM;
static BtnEvent lastBntEvt = {-1,PRESS_END};
volatile static S32		eventsReg = 0;

static Loop appLoop = 
{
		.timeTickMs= loopTimeTick,
		.uart0OnDataRcv = NULL,
		.uart1OnDataRcv = NULL,
		.onButtonEvent = loopOnBtnEvt,
};
/* Private functions ---------------------------------------------------------*/
//volatile static U32 cntr =0;
static void timer1MsCallBack(void)
{
	SET_EVT_FLAG(eventsReg,EVT_TIME_TICK);
}

static void clicCallBack(BtnEvent btnEvt)
{
	lastBntEvt = btnEvt;
	SET_EVT_FLAG(eventsReg,EVT_BTN);
}

static void uartRcvCallBack()
{
	U8 buf[128];
	U32 len =0;
	// ToDo: Echo Back For Now 
	drvUARTRcv(buf, &len);
	drvUARTSend(buf,len);
}

static S32 initTimer(void)
{
	S32 retVal = SUCCESS;

	if(retVal == SUCCESS)
	{
		retVal = drvTimersInit();
	}

	if(retVal == SUCCESS)
	{
		retVal = drvTimerRegister(&timer1Ms,timer1MsCallBack);
	}

	if(retVal == SUCCESS)
	{
		retVal = drvTimerStart(timer1Ms,1,TRUE);
	}

	return retVal;

}

static S32 initDrivers(void)
{
	S32 retVal = SUCCESS;

	retVal = drvInitPlatform();

	if(retVal == SUCCESS)
	{
		retVal = initTimer();
	}

	if(retVal == SUCCESS)
	{
		retVal = drvLedsInit();
	}

	if(retVal == SUCCESS)
	{
		retVal = drvButtonsInit(clicCallBack);
	}

	return retVal;
}

static void loopTimeTick(void)
{
	drvButtonsScan();
	drvLedOnTimeTick();
	middStateProcess(STT_EVT_TICK,NULL);
}

static void loopOnBtnEvt(BtnEvent bntEvt)
{
	middStateProcess(STT_EVT_BTN,(void*)&bntEvt);
}

static void loopApp(Loop* loop)
{
	U32 i =0;

	for(i =0;i<EVT_END;i++)
	{
		if(IS_EVT_OCCUR(eventsReg,i))
		{
			switch(i)
			{
			case EVT_TIME_TICK:
				if(loop->timeTickMs != NULL)
					loop->timeTickMs();
				break;

			case EVT_UART_0:
				if(loop->uart0OnDataRcv != NULL)
					loop->uart0OnDataRcv();
				break;
			case EVT_UART_1:
				if(loop->uart1OnDataRcv != NULL)
					loop->uart1OnDataRcv();
				break;
			case EVT_BTN:
				if(loop->onButtonEvent != NULL)
					loop->onButtonEvent(lastBntEvt);
				break;
			default:
				break;
			}
			RESET_EVT_FLAG(eventsReg,i);
		}
	}
}


/* Public functions ---------------------------------------------------------*/

int main(void)
{
	initDrivers();
	appStateInit();

	drvUARTInit(uartRcvCallBack);

	while(1)
	{
		loopApp(&appLoop);
	}
}
