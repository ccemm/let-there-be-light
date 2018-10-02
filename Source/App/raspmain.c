#include <stdio.h>
#include <unistd.h>
#include "pigpio.h"
#include "time.h"
#include "drvDebug.h"
#include "drvLeds.h"
#include "drvPshButtons.h"


// ---- Button Pin 17
// ---- Led Pin 18

void keyStateChanged(BtnEvent btnEvt )
{
	static bool ledState=false;
	static int prevTick = 0;
	
	if(btnEvt.btnNum == 0 && btnEvt.Evt == PRESS_LONG)
	{
		if(ledState)
		{
			ledState =0;
			drvLedOff();
		}
		else 
		{
			ledState =1;
			drvLedOn();
		}
	}
}

int main(void)
{
	drvPrintf("!!! RPI Program Started !!!\n");
	
	// Set Pin Directions
	if(drvLedsInit() != SUCCESS ||
		drvButtonsInit(keyStateChanged) != SUCCESS )
	{
		drvPrintf("Set Mod Failed \n");
		gpioTerminate();
		return -1;
	}
		
	while(1)
	{
		sleep(1);
	}
	
	gpioTerminate();
	return 0;
}

