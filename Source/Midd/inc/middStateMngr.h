/*******************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
********************************************************************/
#ifndef MID_STATE_MNGR
#define MID_STATE_MNGR

#include "genericTypes.h"
#include "drvPshButtons.h"

typedef enum _EnumStateEvents
{
	STT_EVT_TICK =0,
	STT_EVT_BTN =1,
	STT_EVT_END
}EnumStateEvents;

typedef enum _SttRet
{
	STT_RET_STAY =0,
	STT_RET_NEXT,
	STT_RET_PREV,
	STT_RET_NUM
}SttRet;

typedef struct _State
{
	SttRet (*btn_pressed)(BtnEvent btnEvt);
	SttRet (*onTimeTick)(void);
	struct _State* prevtState;
	struct _State* nextState;
}State;


void middStateInit(State* initialState);
void middStateProcess(EnumStateEvents evt, void* params);

#endif
