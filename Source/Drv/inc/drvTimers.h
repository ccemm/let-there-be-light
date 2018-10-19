/*******************************************************************
 *
 * $Author: Cem AKIN
 * $Date: Kick Start 24.09.2016
 * $Description:
 *
********************************************************************/
#ifndef _DRV_TIMERS_H
#define _DRV_TIMERS_H

#include "drvConfig.h"

#define MAX_TIMERS			(3)
#define	INV_TIMER_NUM		(0xFFFFFFFF)

#if PART_TM4C123GH6PM
#define MIN_PERIOD_MS		(1)
#else
#define MIN_PERIOD_MS		(10)
#endif

typedef int TimerNum;
typedef void (*TimerCalback)(void);

S32 drvTimersInit(void);
S32 drvTimerRegister(TimerNum* tmrNum, TimerCalback callBack);
S32 drvTimerUnRegister(TimerNum tmrNum);
S32 drvTimerStart(TimerNum tmrNum,U32 timeoutMs,BOOL isContiniues);
S32 drvTimerStop(TimerNum tmrNum);
S32 drvSetEnabe(BOOL enable);


#endif
