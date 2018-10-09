/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date: 24.09.2016
 * $Description:
 *
*******************************************************************************/
#include "drvConfig.h"

#include "drvTimers.h"
#include "pigpio.h"


/* Private define ------------------------------------------------------------*/
#define MIN_PERIOD_MS		(10)
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
/* Public functions ---------------------------------------------------------*/

void Timer0(void*)
{
	U32 i =0;

	for(i =0; i<MAX_TIMERS;i++)
	{
		if( (timers[i].callBack != NULL) && (timers[i].isRunnig))
		{
			timers[i].cntr+=1;
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

	if( gpioSetTimerFuncEx(0, 10, Timer0, (void*)NULL) <0)
	{
		retVal = FAILURE;
	}
	else
	{
		isInited = TRUE;
	}
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

	if( (isInited != TRUE) || ((timeoutMs% MIN_PERIOD_MS) != 0) || (timeoutMs == 0) )
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
	S32 retVal = FAILURE;
	// ToDo:
	return retVal;
}
