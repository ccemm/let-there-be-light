/*******************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
********************************************************************/
#ifndef _DRV_BUTTONS_H
#define _DRV_BUTTONS_H

#include "drvConfig.h"

typedef enum _ClickEvt
{
	PRESS_LONG = 0,
	PRESS_SHORT = 1,
	PRESS_END
}ClickEvt;

typedef struct _BtnEvent
{
	int btnNum;
	ClickEvt Evt;
}BtnEvent;

typedef  void(*ClickCallback)(BtnEvent evt);

S32 drvButtonsInit(ClickCallback cBack);
S32 drvButtonsScan(void);

#endif
