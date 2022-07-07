#if 0

/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date: 24.09.2016
 * $Description:
 *
*******************************************************************************/
#include "drvConfig.h"

#include "timer.h"
#include "interrupt.h"

#include "drvTimers.h"
#include "stm32l100xc.h"

/* Private define ------------------------------------------------------------*/
#define MIN_PERIOD_MS		(1)
/* Private typedef -----------------------------------------------------------*/
typedef struct _Timer
{
	TimerCalback callBack;
	U32 timeOut;
	U32 cntr;
	BOOL isRunnig;
	BOOL isContiniues;
}Timer;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static BOOL isInited = FALSE;
volatile Timer timers[MAX_TIMERS];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void initHardwareTimer(void)
{
	// Enable Timer2 Periphereal
	BIT_SET(RCC->APB1ENR,0);

	// Timer0 Split Mode Two 16 bit timers
	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);

	// Set Timer Inrerrupt period As MIN_PERIOD_MS
	TimerLoadSet(TIMER0_BASE, TIMER_B, (SysCtlClockGet() / (1000*MIN_PERIOD_MS)));
	 // Clear the timer interrupt flag.
	TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
	// Enable All Interrupts
	IntMasterEnable();

	// Timer generates Interrup For Timer0 TimerB Interrupt
	TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

	//Enable Ineterrupt TIMER0B interrupt
	IntEnable(INT_TIMER0B);

	// Enable Timer
	TimerEnable(TIMER0_BASE, TIMER_B);
}
/* Public functions ---------------------------------------------------------*/
void Timer0BIntHandler(void)
{
	U32 i =0;
    // Clear the timer interrupt flag.
    TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

	for(i =0; i<MAX_TIMERS;i++)
	{
		if( (timers[i].callBack != NULL) && (timers[i].isRunnig))
		{
			timers[i].cntr+=MIN_PERIOD_MS;
			if(timers[i].cntr >= timers[i].timeOut)
			{
				if(timers[i].isContiniues == FALSE)
				{
					timers[i].isRunnig = FALSE;
				}
				timers[i].callBack();
				timers[i].cntr = 0;
			}
		}
	}
}

/**
  * @brief  Initializes Timer Module, Also Configure and Enable Hardware Timer
  * @param  None
  * @retval SUCCESS If Operation Sucessfull FAILURE For all other cases
  */
S32 drvTimersInit(void)
{
	S32 retVal = SUCCESS;
	U32 i =0;
	// init timers
	for(i =0;i<MAX_TIMERS;i++)
	{
		timers[i].callBack = NULL;
		timers[i].timeOut = 0;
		timers[i].cntr = 0;
		timers[i].isRunnig = FALSE;
		timers[i].isContiniues = FALSE;
	}
	initHardwareTimer();
	isInited = TRUE;
	return retVal;
}

/**
  * @brief  Register Timer, Timer Timeout Must be a multiply of MIN_PERIOD_MS
  * @param  None
  * @retval SUCCESS or FAILURE
  * @wanring: !!! Registered Callback Function Will Be called In an Interrupt !!!!
  */
S32 drvTimerRegister(TimerNum* tmrNum, TimerCalback callBack)
{
	S32 retVal = SUCCESS;
	U32 i =0;

	if(isInited != TRUE)
	{
		retVal = FAILURE;
	}

	if(retVal == SUCCESS)
	{
		retVal = FAILURE;
		for(i =0;i<MAX_TIMERS;i++)
		{
			if( timers[i].callBack == NULL)
			{
				timers[i].callBack = callBack;
				timers[i].isRunnig = FALSE;
				timers[i].isContiniues = FALSE;
				*tmrNum =i;
				retVal = SUCCESS;
				break;
			}
		}
	}
	return retVal;
}

/**
  * @brief  Unregister Soft Timer
  * @param  None
  * @retval SUCCESS or FAILURE
  */
S32 drvTimerUnRegister(TimerNum tmrNum)
{
	S32 retVal = SUCCESS;
	timers[tmrNum].callBack = NULL;
	timers[tmrNum].timeOut = FALSE;
	timers[tmrNum].isRunnig = FALSE;
	return retVal;
}

/**
  * @brief  Starts Soft
  * @param  None
  * @retval SUCCESS or FAILURE
  */
S32 drvTimerStart(TimerNum tmrNum,U32 timeoutMs, BOOL isContiniues)
{
	S32 retVal = SUCCESS;

	if( (isInited != TRUE) || ( (timeoutMs% MIN_PERIOD_MS) != 0) || (timeoutMs == 0) )
	{
		retVal = FAILURE;
	}

	if(retVal == SUCCESS)
	{
		if( timers[tmrNum].callBack != NULL)
		{
			timers[tmrNum].isRunnig = TRUE;
			timers[tmrNum].timeOut = (timeoutMs/MIN_PERIOD_MS) ;
			timers[tmrNum].cntr = 0;
			timers[tmrNum].isContiniues = isContiniues;
		}
		else
		{
			retVal = FAILURE;
	}
	}
	return retVal;
}

/**
  * @brief  Initializes Timers
  * @param  None
  * @retval SUCCESS or FAILURE
  */
S32 drvTimerStop(TimerNum tmrNum)
{
	S32 retVal = SUCCESS;
	if( timers[tmrNum].callBack != NULL)
	{
		timers[tmrNum].isRunnig = FALSE;
	}
	else
	{
		retVal = FAILURE;
	}
	return retVal;
}
/**
  * @brief  Enables Disables All Timers, Timers are Enabled after Init by default
  * @param  None
  * @retval SUCCESS If Operation Sucessfull FAILURE For all other cases
  */
S32 drvSetEnabe(BOOL enable)
{
	S32 retVal = SUCCESS;
	if(enable)
	{
		TimerEnable(TIMER0_BASE, TIMER_B);;
	}
	else
	{
		TimerDisable(TIMER0_BASE, TIMER_B);
	}
	return retVal;
}



#endif 

