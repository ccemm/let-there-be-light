/******************************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
*******************************************************************************/
#include "middStateMngr.h"

static State* crrState;

void middStateInit(State* initState)
{
	crrState = initState;
}

void middStateProcess(EnumStateEvents evt, void* params)
{
	SttRet sttRetVal = STT_RET_STAY;
	switch(evt)
	{
	case STT_EVT_TICK:
		if(crrState->onTimeTick != NULL)
		{
			sttRetVal = crrState->onTimeTick();
		}
	break;
	case STT_EVT_BTN:
		if(crrState->btn_pressed != NULL)
		{
			sttRetVal = crrState->btn_pressed(*(BtnEvent*)params);
		}
	break;
	default:
		break;
	}

	if(sttRetVal == STT_RET_NEXT)
	{
		crrState = crrState->nextState;
	}
	else if(sttRetVal == STT_RET_PREV)
	{
		crrState = crrState->prevtState;
	}
}
