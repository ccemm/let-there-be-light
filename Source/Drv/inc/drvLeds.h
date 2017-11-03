/*******************************************************************
 *
 * $Author: Cem AKIN
 * $Date: Kick Start 24.09.2016
 * $Description:
 *
********************************************************************/
#ifndef _DRV_LEDS_H
#define _DRV_LEDS_H

#include "drvConfig.h"

S32 drvLedsInit(void);
void drvLedOn(void);
void drvLedOff(void);
void drvLedToggle(void);
void drvLedOnTimeTick(void);
int drvLedSetBlinkMode(LedBlinkMode blnkMode);



#endif


